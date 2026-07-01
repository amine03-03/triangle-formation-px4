"""Plain dataclasses shared across the core. No ROS imports on purpose."""
from __future__ import annotations
from dataclasses import dataclass, field
import numpy as np


@dataclass
class LeaderState:
    """Virtual-leader sample in the shared world NED frame."""
    position: np.ndarray = field(default_factory=lambda: np.zeros(3))  # [N,E,D]
    velocity: np.ndarray = field(default_factory=lambda: np.zeros(3))
    yaw: float = 0.0        # rad, NED heading (0 = North)
    yaw_rate: float = 0.0   # rad/s


@dataclass
class DroneState:
    """Live per-drone state in the shared world NED frame."""
    index: int
    position: np.ndarray = field(default_factory=lambda: np.zeros(3))
    yaw: float = 0.0
    valid: bool = False
