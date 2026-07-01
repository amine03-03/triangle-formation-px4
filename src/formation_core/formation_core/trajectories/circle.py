from __future__ import annotations
import numpy as np
from .base import TrajectoryBase
from ..types import LeaderState


class CircleTrajectory(TrajectoryBase):
    """Example of 'just add a file': a horizontal circle at constant speed.
    Demonstrates that any analytic path plugs in behind the same interface."""

    def __init__(self, center, radius=10.0, speed=2.0, altitude=5.0, laps=1.0):
        self.c = np.asarray(center, dtype=float)  # [north, east]
        self.r = float(radius)
        self.speed = float(speed)
        self.down = -float(altitude)
        self.laps = float(laps)
        self.omega = self.speed / self.r if self.r > 1e-6 else 0.0

    def duration(self) -> float:
        return self.laps * 2.0 * np.pi / self.omega if self.omega > 1e-9 else 0.0

    def sample(self, t: float) -> LeaderState:
        th = self.omega * t
        n = self.c[0] + self.r * np.cos(th)
        e = self.c[1] + self.r * np.sin(th)
        vn = -self.r * self.omega * np.sin(th)
        ve = self.r * self.omega * np.cos(th)
        yaw = float(np.arctan2(ve, vn))
        return LeaderState(position=np.array([n, e, self.down]),
                           velocity=np.array([vn, ve, 0.0]),
                           yaw=yaw, yaw_rate=self.omega)
