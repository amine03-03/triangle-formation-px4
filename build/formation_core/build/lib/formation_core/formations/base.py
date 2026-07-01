from __future__ import annotations
from abc import ABC, abstractmethod
import numpy as np


class FormationBase(ABC):
    """A formation maps slot index -> body-frame offset (x-fwd, y-right, z-down)
    relative to the virtual leader. Offsets are rotated into world NED by the
    leader yaw at run time."""

    @abstractmethod
    def offsets(self) -> dict[int, np.ndarray]:
        ...

    @property
    def n_slots(self) -> int:
        return len(self.offsets())

    def max_radius(self) -> float:
        """Largest |offset| in the xy-plane -- drives the turn-rate limit."""
        return max(float(np.hypot(o[0], o[1])) for o in self.offsets().values())
