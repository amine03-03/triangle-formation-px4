"""Frame helpers. Everything in this stack lives in NED to match PX4's
TrajectorySetpoint / VehicleLocalPosition, so no ENU<->NED juggling leaks
into the control math. Body frame convention: x-forward, y-right, z-down."""
from __future__ import annotations
import numpy as np


def rot_z_ned(psi: float) -> np.ndarray:
    """3x3 rotation about the NED down-axis by heading psi (rad).

    Maps a body vector (x-fwd, y-right, z-down) into world NED. At psi=0 the
    body faces North, so forward -> +North and right -> +East.
    """
    c, s = np.cos(psi), np.sin(psi)
    return np.array([[c, -s, 0.0],
                     [s,  c, 0.0],
                     [0.0, 0.0, 1.0]])


def heading_from_velocity_ned(vel_ned: np.ndarray, fallback: float = 0.0) -> float:
    """NED heading from a velocity vector: atan2(East, North)."""
    n, e = float(vel_ned[0]), float(vel_ned[1])
    if abs(n) < 1e-6 and abs(e) < 1e-6:
        return fallback
    return float(np.arctan2(e, n))


# Gazebo world is ENU; PX4 local is NED. Only needed when talking to Gazebo
# (e.g. building a spawn pose string), never inside the control loop.
def ned_to_enu_xy(north: float, east: float) -> tuple[float, float]:
    """world NED (north, east) -> Gazebo ENU (x=east, y=north)."""
    return east, north
