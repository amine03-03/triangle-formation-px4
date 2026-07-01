from __future__ import annotations
import numpy as np
from .base import FormationFeedback


class ConsensusFeedback(FormationFeedback):
    """Displacement-based consensus term:

        u_i = -k * sum_j [ (p_i - p_j) - (o_i - o_j) ]

    where p are world positions and o are the (already yaw-rotated) desired
    world offsets. Drives relative geometry toward the target triangle using
    only neighbor information. Output magnitude is clipped so it stays a gentle
    correction on top of the position setpoint (see design note re: position
    vs velocity setpoints).
    """

    def __init__(self, gain: float = 0.8, max_correction: float = 1.5):
        self.k = float(gain)
        self.max_c = float(max_correction)

    def compute(self, index, self_pos, neighbor_pos, desired_offsets):
        o_i = desired_offsets[index]
        acc = np.zeros(3)
        for j, p_j in neighbor_pos.items():
            if j == index:
                continue
            rel_actual = self_pos - p_j
            rel_desired = o_i - desired_offsets[j]
            acc += (rel_actual - rel_desired)
        u = -self.k * acc
        n = float(np.linalg.norm(u))
        if n > self.max_c and n > 1e-9:
            u *= self.max_c / n
        return u
