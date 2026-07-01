from __future__ import annotations
import numpy as np
from .base import FormationFeedback


class ZeroFeedback(FormationFeedback):
    """v1 default: rely purely on the virtual-leader reference."""

    def compute(self, index, self_pos, neighbor_pos, desired_offsets):
        return np.zeros(3)
