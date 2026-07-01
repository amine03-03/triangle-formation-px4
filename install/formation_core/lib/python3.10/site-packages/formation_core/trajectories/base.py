from __future__ import annotations
from abc import ABC, abstractmethod
from ..types import LeaderState


class TrajectoryBase(ABC):
    """A trajectory turns mission time -> virtual-leader state in world NED.
    Add a new trajectory type by subclassing this and registering it."""

    @abstractmethod
    def sample(self, t: float) -> LeaderState:
        """Return the leader state at mission time t (seconds, t>=0)."""

    @abstractmethod
    def duration(self) -> float:
        """Nominal completion time (seconds)."""

    def is_finished(self, t: float) -> bool:
        return t >= self.duration()
