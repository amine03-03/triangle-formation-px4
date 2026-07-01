"""Per-drone node: owns one PX4 instance's offboard control.

Responsibilities:
  * stream the offboard heartbeat + a position/yaw setpoint at control_rate,
  * compute that setpoint as   leader_slot + consensus_correction,
  * run a robust arm/offboard handshake (retries until PX4 confirms),
  * publish its own DroneState (world NED) and consume peers' for feedback,
  * on LAND phase, hand control back to PX4 auto-land.

The virtual leader and phase arrive from the coordinator via LeaderState.
"""
from __future__ import annotations
import numpy as np
import rclpy
from rclpy.node import Node

from formation_interfaces.msg import LeaderState, DroneState
from formation_core.formations.triangle import TriangleFormation
from formation_core.feedback.registry import build_feedback
from formation_core.math.frames import rot_z_ned
from . import phases
from .px4_io import Px4Io


class DroneInterface(Node):
    def __init__(self):
        super().__init__('drone_interface')
        # ---- parameters ----
        self.declare_parameter('index', 0)
        self.declare_parameter('namespace', 'px4_1')
        self.declare_parameter('sys_id', 2)
        self.declare_parameter('spawn_offset_ned', [0.0, 0.0, 0.0])
        self.declare_parameter('side', 4.0)
        self.declare_parameter('altitude', 5.0)
        self.declare_parameter('control_rate', 50.0)
        self.declare_parameter('warmup_s', 1.0)
        self.declare_parameter('feedback_type', 'zero')       # 'zero' | 'consensus'
        self.declare_parameter('feedback_gain', 0.8)
        self.declare_parameter('feedback_max_correction', 1.5)

        self.index = int(self.get_parameter('index').value)
        ns = str(self.get_parameter('namespace').value)
        sys_id = int(self.get_parameter('sys_id').value)
        spawn = list(self.get_parameter('spawn_offset_ned').value)
        side = float(self.get_parameter('side').value)
        self.altitude = float(self.get_parameter('altitude').value)
        rate = float(self.get_parameter('control_rate').value)
        self.warmup = float(self.get_parameter('warmup_s').value)

        self.offsets = TriangleFormation(side=side).offsets()   # body-frame slots
        self.feedback = build_feedback({
            'type': str(self.get_parameter('feedback_type').value),
            'gain': float(self.get_parameter('feedback_gain').value),
            'max_correction': float(self.get_parameter('feedback_max_correction').value),
        })

        self.io = Px4Io(self, ns, sys_id, spawn)
        self.spawn = np.asarray(spawn, dtype=float)

        # ---- formation comms ----
        self.leader = None                # latest LeaderState
        self.peers: dict[int, np.ndarray] = {}   # idx -> world pos
        self.create_subscription(LeaderState, '/formation/leader_state', self._on_leader, 10)
        self.create_subscription(DroneState, '/formation/drone_state', self._on_peer, 10)
        self._pub_state = self.create_publisher(DroneState, '/formation/drone_state', 10)

        self._tick = 0
        self._land_sent = False
        self.dt = 1.0 / rate
        self.create_timer(self.dt, self._loop)
        self.get_logger().info(f'drone_interface idx={self.index} ns={ns} sys_id={sys_id} up')

    # ---------- callbacks ----------
    def _on_leader(self, msg): self.leader = msg

    def _on_peer(self, msg):
        if msg.valid:
            self.peers[msg.index] = np.array(msg.position, dtype=float)

    # ---------- helpers ----------
    def _world_offsets(self, yaw):
        R = rot_z_ned(yaw)
        return {i: R @ o for i, o in self.offsets.items()}

    def _slot(self, leader_pos, yaw):
        return leader_pos + rot_z_ned(yaw) @ self.offsets[self.index]

    def _publish_own_state(self, pos, yaw, valid):
        m = DroneState()
        m.timestamp = int(self.get_clock().now().nanoseconds / 1000)
        m.index = self.index
        m.position = [float(pos[0]), float(pos[1]), float(pos[2])]
        m.yaw = float(yaw)
        m.valid = bool(valid)
        self._pub_state.publish(m)

    # ---------- main loop ----------
    def _loop(self):
        self._tick += 1
        pos_w, heading, valid = self.io.world_position()
        self._publish_own_state(pos_w, heading, valid)

        phase = self.leader.phase if self.leader is not None else phases.INIT

        # LAND: stop fighting PX4, trigger auto-land once.
        if phase == phases.LAND:
            if not self._land_sent:
                self.io.land()
                self._land_sent = True
                self.get_logger().info(f'idx={self.index} LAND commanded')
            return

        # Always stream heartbeat (offboard requires it >2 Hz).
        self.io.publish_offboard_heartbeat()

        # Determine setpoint.
        if self.leader is None:
            # No leader yet: hold above spawn at target altitude (also takeoff).
            sp = self.spawn.copy()
            sp[2] = -self.altitude
            yaw_cmd = heading
        else:
            leader_pos = np.array(self.leader.position, dtype=float)
            yaw_cmd = float(self.leader.yaw)
            slot = self._slot(leader_pos, yaw_cmd)
            corr = np.zeros(3)
            if valid and self.peers:
                corr = self.feedback.compute(
                    self.index, pos_w, self.peers, self._world_offsets(yaw_cmd))
            sp = slot + corr

        self.io.publish_setpoint_world(sp, yaw_cmd)

        # Robust handshake: after warmup, keep requesting offboard+arm until PX4
        # confirms both. Re-requests are harmless if already satisfied.
        if self._tick * self.dt >= self.warmup and self._tick % 25 == 0:
            if not self.io.offboard:
                self.io.set_offboard()
            if not self.io.armed:
                self.io.arm()


def main():
    rclpy.init()
    node = DroneInterface()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
