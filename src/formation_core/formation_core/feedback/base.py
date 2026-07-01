from __future__ import annotations
from abc import ABC, abstractmethod
import numpy as np


class FormationFeedback(ABC):
    """Inter-agent correction added on top of the virtual-leader slot.

    compute() returns a world-NED correction vector (m) for `index`. v1 ships
    ZeroFeedback (returns 0); swapping in ConsensusFeedback needs no plumbing
    change because peer states already flow through the system.
    """

    @abstractmethod
    def compute(self, index: int,
                self_pos: np.ndarray,
                neighbor_pos: dict[int, np.ndarray],
                desired_offsets: dict[int, np.ndarray]) -> np.ndarray:
        ...
