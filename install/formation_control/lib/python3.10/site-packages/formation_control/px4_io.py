"""Thin per-drone PX4 interface over uXRCE-DDS.

Owns the publishers/subscribers for ONE namespaced PX4 instance
(/px4_<i>/fmu/...) and encapsulates:
  * the WORLD<->LOCAL frame conversion via a fixed spawn_offset, because each
    PX4 EKF origin sits at that vehicle's own spawn point (see design note),
  * the offboard heartbeat + position setpoint,
  * arm / set-offboard / land VehicleCommands.

All public geometry is in the shared WORLD NED frame; conversion to the
vehicle-local NED frame happens only at the setpoint boundary.
"""
from __future__ import annotations
import numpy as np
from rclpy.qos import QoSProfile, ReliabilityPolicy, DurabilityPolicy, HistoryPolicy

from px4_msgs.msg import (OffboardControlMode, TrajectorySetpoint,
                          VehicleCommand, VehicleLocalPosition, VehicleStatus)

# Canonical PX4 <-> ROS 2 QoS (best-effort, keep-last-1). Matches the PX4
# offboard example; mismatching this is the usual "topic connects but no data".
PX4_QOS = QoSProfile(reliability=ReliabilityPolicy.BEST_EFFORT,
                     durability=DurabilityPolicy.TRANSIENT_LOCAL,
                     history=HistoryPolicy.KEEP_LAST, depth=1)


class Px4Io:
    def __init__(self, node, namespace: str, sys_id: int, spawn_offset_ned):
        self.node = node
        self.sys_id = int(sys_id)
        self.spawn_offset = np.asarray(spawn_offset_ned, dtype=float)  # world = local + offset
        ns = namespace.strip('/')
        p = f'/{ns}/fmu' if ns else '/fmu'

        self._pub_ocm = node.create_publisher(OffboardControlMode, f'{p}/in/offboard_control_mode', PX4_QOS)
        self._pub_sp = node.create_publisher(TrajectorySetpoint, f'{p}/in/trajectory_setpoint', PX4_QOS)
        self._pub_cmd = node.create_publisher(VehicleCommand, f'{p}/in/vehicle_command', PX4_QOS)
        node.create_subscription(VehicleLocalPosition, f'{p}/out/vehicle_local_position_v1', self._on_lpos, PX4_QOS)
        node.create_subscription(VehicleStatus, f'{p}/out/vehicle_status', self._on_status, PX4_QOS)

        self._lpos = None
        self._status = None

    # ---------- telemetry ----------
    def _on_lpos(self, msg): self._lpos = msg
    def _on_status(self, msg): self._status = msg

    def world_position(self):
        """Return (pos_world_ned[3], heading, valid)."""
        m = self._lpos
        if m is None or not (m.xy_valid and m.z_valid):
            return np.zeros(3), 0.0, False
        local = np.array([m.x, m.y, m.z], dtype=float)
        return local + self.spawn_offset, float(m.heading), True

    @property
    def armed(self):
        return self._status is not None and self._status.arming_state == VehicleStatus.ARMING_STATE_ARMED

    @property
    def offboard(self):
        return self._status is not None and self._status.nav_state == VehicleStatus.NAVIGATION_STATE_OFFBOARD

    # ---------- outputs ----------
    def _now_us(self):
        return int(self.node.get_clock().now().nanoseconds / 1000)

    def publish_offboard_heartbeat(self):
        m = OffboardControlMode()
        m.timestamp = self._now_us()
        m.position = True
        m.velocity = m.acceleration = m.attitude = m.body_rate = False
        m.thrust_and_torque = m.direct_actuator = False
        self._pub_ocm.publish(m)

    def publish_setpoint_world(self, pos_world_ned, yaw):
        local = np.asarray(pos_world_ned, dtype=float) - self.spawn_offset
        m = TrajectorySetpoint()
        m.timestamp = self._now_us()
        m.position = [float(local[0]), float(local[1]), float(local[2])]
        nan = float('nan')
        m.velocity = [nan, nan, nan]
        m.acceleration = [nan, nan, nan]
        m.jerk = [nan, nan, nan]
        m.yaw = float(yaw)
        m.yawspeed = nan
        self._pub_sp.publish(m)

    def _send_cmd(self, command, p1=0.0, p2=0.0):
        m = VehicleCommand()
        m.timestamp = self._now_us()
        m.command = int(command)
        m.param1 = float(p1)
        m.param2 = float(p2)
        m.target_system = self.sys_id
        m.target_component = 1
        m.source_system = self.sys_id
        m.source_component = 1
        m.from_external = True
        self._pub_cmd.publish(m)

    def arm(self):
        self._send_cmd(VehicleCommand.VEHICLE_CMD_COMPONENT_ARM_DISARM, 1.0)

    def set_offboard(self):
        # base_mode custom enabled (1) + PX4 custom main mode OFFBOARD (6)
        self._send_cmd(VehicleCommand.VEHICLE_CMD_DO_SET_MODE, 1.0, 6.0)

    def land(self):
        self._send_cmd(VehicleCommand.VEHICLE_CMD_NAV_LAND)
