from __future__ import annotations
import numpy as np
from .base import TrajectoryBase
from ..types import LeaderState
from ..math.frames import heading_from_velocity_ned


class WaypointTrajectory(TrajectoryBase):
    """Piecewise-linear waypoint follower at constant cruise speed.

    waypoints: (M,3) array of world-NED points [north, east, down].
    Position advances by arc length s = cruise_speed * t. Heading is the raw
    path tangent; corner smoothing is applied downstream (coordinator rate-
    limits yaw), keeping this class stateless and unit-testable.
    """

    def __init__(self, waypoints, cruise_speed: float = 2.0):
        wp = np.asarray(waypoints, dtype=float)
        assert wp.ndim == 2 and wp.shape[1] == 3 and len(wp) >= 2, \
            "waypoints must be (M>=2, 3) in NED"
        self.wp = wp
        self.cruise = float(cruise_speed)
        seg = np.diff(wp, axis=0)
        self.seg_len = np.linalg.norm(seg, axis=1)
        # guard against duplicate waypoints
        self.seg_dir = np.zeros_like(seg)
        nz = self.seg_len > 1e-9
        self.seg_dir[nz] = seg[nz] / self.seg_len[nz, None]
        self.cum = np.concatenate([[0.0], np.cumsum(self.seg_len)])
        self.total_len = float(self.cum[-1])

    def duration(self) -> float:
        return self.total_len / self.cruise if self.cruise > 1e-9 else 0.0

    def sample(self, t: float) -> LeaderState:
        s = max(0.0, self.cruise * t)
        if s >= self.total_len:  # clamp to final waypoint, stationary
            return LeaderState(position=self.wp[-1].copy(),
                               velocity=np.zeros(3), yaw=self._last_yaw(),
                               yaw_rate=0.0)
        k = int(np.searchsorted(self.cum, s, side='right') - 1)
        k = min(max(k, 0), len(self.seg_len) - 1)
        local = s - self.cum[k]
        pos = self.wp[k] + self.seg_dir[k] * local
        vel = self.seg_dir[k] * self.cruise
        yaw = heading_from_velocity_ned(vel, fallback=self._last_yaw())
        return LeaderState(position=pos, velocity=vel, yaw=yaw, yaw_rate=0.0)

    def _last_yaw(self) -> float:
        for k in range(len(self.seg_len) - 1, -1, -1):
            if self.seg_len[k] > 1e-9:
                return heading_from_velocity_ned(self.seg_dir[k])
        return 0.0
