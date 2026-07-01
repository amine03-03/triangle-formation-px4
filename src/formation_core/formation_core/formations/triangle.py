from __future__ import annotations
import numpy as np
from .base import FormationBase


class TriangleFormation(FormationBase):
    """Equilateral triangle, apex forward, virtual leader at the CENTROID.

    Body frame x-forward, y-right, z-down. For side length s the centroid-to-
    apex distance is s/sqrt(3) and the rear vertices sit at (-s/(2 sqrt3), +/- s/2).
    All three slots share the circumradius s/sqrt(3) ~= 0.577 s.
    """

    def __init__(self, side: float = 4.0):
        self.side = float(side)

    def offsets(self) -> dict[int, np.ndarray]:
        s = self.side
        apex_x = s / np.sqrt(3.0)
        rear_x = -s / (2.0 * np.sqrt(3.0))
        half = s / 2.0
        return {
            0: np.array([apex_x, 0.0, 0.0]),    # apex (lead)
            1: np.array([rear_x, +half, 0.0]),  # rear-right
            2: np.array([rear_x, -half, 0.0]),  # rear-left
        }
