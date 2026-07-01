"""Yaw utilities. The corner-snapping problem from the design phase is solved
here: rate_limit_yaw() slews the commanded heading toward the target instead
of letting it jump at a waypoint corner."""
from __future__ import annotations
import numpy as np


def wrap_pi(a: float) -> float:
    return float((a + np.pi) % (2.0 * np.pi) - np.pi)


def shortest_angular_distance(frm: float, to: float) -> float:
    return wrap_pi(to - frm)


def rate_limit_yaw(current: float, target: float, max_rate: float, dt: float) -> float:
    """Move `current` toward `target` by at most max_rate*dt (rad)."""
    err = shortest_angular_distance(current, target)
    max_step = max_rate * dt
    if err > max_step:
        err = max_step
    elif err < -max_step:
        err = -max_step
    return wrap_pi(current + err)
