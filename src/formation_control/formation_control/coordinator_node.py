"""Coordinator: owns the MISSION only.

  * ticks the virtual-leader clock and samples the (pluggable) trajectory,
  * rate-limits the leader yaw so heading-aligned corners stay feasible,
  * runs the phase state machine INIT -> ASSEMBLE -> ENGAGE -> HOLD -> LAND,
  * broadcasts one LeaderState (pose + yaw + phase) that every drone offsets
    from; it does NOT command drones directly (they own their own control).

Convergence for ASSEMBLE->ENGAGE is decided from the drones' DroneState.
"""
from __future__ import annotations
import numpy as np
import rclpy
from rclpy.node import Node

from formation_interfaces.msg import LeaderState, DroneState
from formation_core.formations.triangle import TriangleFormation
from formation_core.trajectories.registry import build_trajectory
from formation_core.math.frames import rot_z_ned
from formation_core.math.yaw import rate_limit_yaw
from . import phases


class Coordinator(Node):
    def __init__(self):
        super().__init__('coordinator')
        self.declare_parameter('n_drones', 3)
        self.declare_parameter('side', 4.0)
        self.declare_parameter('altitude', 5.0)
        self.declare_parameter('control_rate', 50.0)
        self.declare_parameter('yaw_rate_max', 0.5)          # rad/s
        self.declare_parameter('assemble_tol', 0.5)          # m
        self.declare_parameter('assemble_settle_s', 3.0)
        self.declare_parameter('init_hold_s', 3.0)
        self.declare_parameter('final_hold_s', 5.0)
        # trajectory config as flat params (see bringup yaml)
        self.declare_parameter('trajectory_type', 'waypoint')
        self.declare_parameter('cruise_speed', 2.0)
        # waypoints flattened [n0,e0,n1,e1,...] (ROS params dislike nested lists)
        self.declare_parameter('waypoints_flat', [0.0, 0.0, 10.0, 0.0, 10.0, 10.0, 0.0, 10.0, 0.0, 0.0])

        self.n = int(self.get_parameter('n_drones').value)
        side = float(self.get_parameter('side').value)
        self.altitude = float(self.get_parameter('altitude').value)
        rate = float(self.get_parameter('control_rate').value)
        self.yaw_rate_max = float(self.get_parameter('yaw_rate_max').value)
        self.tol = float(self.get_parameter('assemble_tol').value)
        self.settle = float(self.get_parameter('assemble_settle_s').value)
        self.init_hold = float(self.get_parameter('init_hold_s').value)
        self.final_hold = float(self.get_parameter('final_hold_s').value)

        self.offsets = TriangleFormation(side=side).offsets()

        wp_flat = list(self.get_parameter('waypoints_flat').value)
        wp = np.array(wp_flat, dtype=float).reshape(-1, 2)
        self.traj = build_trajectory({
            'type': str(self.get_parameter('trajectory_type').value),
            'cruise_speed': float(self.get_parameter('cruise_speed').value),
            'altitude': self.altitude,
            'waypoints': wp,
        })

        # start pose = first sample; leader initially faces its first heading
        start = self.traj.sample(0.0)
        self.leader_pos = start.position.copy()
        self.leader_vel = np.zeros(3)
        self.smoothed_yaw = self.traj.sample(1e-3).yaw   # heading into first leg
        self.leader_yaw_rate = 0.0

        self.phase = phases.INIT
        self.states: dict[int, np.ndarray] = {}
        self.create_subscription(DroneState, '/formation/drone_state', self._on_state, 10)
        self._pub = self.create_publisher(LeaderState, '/formation/leader_state', 10)

        self.dt = 1.0 / rate
        self.t0 = self.get_clock().now()
        self.phase_t0 = self.t0
        self.engage_t0 = None
        self.converged_since = None
        self.create_timer(self.dt, self._loop)
        self.get_logger().info(f'coordinator up: {self.n} drones, phase INIT')

    def _on_state(self, msg):
        if msg.valid:
            self.states[msg.index] = np.array(msg.position, dtype=float)

    def _elapsed(self, since):
        return (self.get_clock().now() - since).nanoseconds * 1e-9

    def _set_phase(self, p):
        if p != self.phase:
            self.get_logger().info(f'phase {phases.NAMES[self.phase]} -> {phases.NAMES[p]}')
            self.phase = p
            self.phase_t0 = self.get_clock().now()

    def _slots(self, yaw):
        R = rot_z_ned(yaw)
        return {i: self.leader_pos + R @ o for i, o in self.offsets.items()}

    def _all_converged(self, yaw):
        if len(self.states) < self.n:
            return False
        slots = self._slots(yaw)
        for i, slot in slots.items():
            if i not in self.states:
                return False
            if np.linalg.norm(self.states[i] - slot) > self.tol:
                return False
        return True

    def _loop(self):
        # ----- update leader pose per phase -----
        if self.phase == phases.ENGAGE:
            t = self._elapsed(self.engage_t0)
            ls = self.traj.sample(t)
            self.leader_pos = ls.position.copy()
            self.leader_vel = ls.velocity.copy()
            self.smoothed_yaw = rate_limit_yaw(self.smoothed_yaw, ls.yaw,
                                               self.yaw_rate_max, self.dt)
            if self.traj.is_finished(t):
                self._set_phase(phases.HOLD)
        # INIT/ASSEMBLE/HOLD: leader stationary at current leader_pos

        # ----- broadcast -----
        m = LeaderState()
        m.timestamp = int(self.get_clock().now().nanoseconds / 1000)
        m.position = [float(v) for v in self.leader_pos]
        m.velocity = [float(v) for v in self.leader_vel]
        m.yaw = float(self.smoothed_yaw)
        m.yaw_rate = float(self.leader_yaw_rate)
        m.phase = int(self.phase)
        self._pub.publish(m)

        # ----- phase transitions -----
        if self.phase == phases.INIT:
            if self._elapsed(self.phase_t0) >= self.init_hold:
                self._set_phase(phases.ASSEMBLE)

        elif self.phase == phases.ASSEMBLE:
            slots = self._slots(self.smoothed_yaw)
            errs = {i: float(np.linalg.norm(self.states[i] - slots[i]))
                    for i in slots if i in self.states}
            self.get_logger().info(
                f'ASSEMBLE states={len(self.states)}/{self.n} errs={errs}',
                throttle_duration_sec=1.0)
            if self._all_converged(self.smoothed_yaw):
                if self.converged_since is None:
                    self.converged_since = self.get_clock().now()
                elif self._elapsed(self.converged_since) >= self.settle:
                    self.engage_t0 = self.get_clock().now()
                    self._set_phase(phases.ENGAGE)
            else:
                self.converged_since = None

        elif self.phase == phases.HOLD:
            if self._elapsed(self.phase_t0) >= self.final_hold:
                self._set_phase(phases.LAND)


def main():
    rclpy.init()
    node = Coordinator()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
