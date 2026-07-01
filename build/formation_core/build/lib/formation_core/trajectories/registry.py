"""Name -> trajectory factory. Config selects a trajectory by string, so new
types are opt-in without touching the nodes."""
from __future__ import annotations
import numpy as np
from .waypoint import WaypointTrajectory
from .circle import CircleTrajectory


def build_trajectory(cfg: dict):
    """cfg example (waypoint):
        {type: waypoint, cruise_speed: 2.0, altitude: 5.0,
         waypoints: [[0,0],[10,0],[10,10],[0,10],[0,0]]}   # [north, east]
       cfg example (circle):
        {type: circle, center: [0,0], radius: 10, speed: 2, altitude: 5, laps: 1}
    """
    t = cfg['type']
    if t == 'waypoint':
        alt = float(cfg.get('altitude', 5.0))
        wp2d = np.asarray(cfg['waypoints'], dtype=float)
        down = np.full((len(wp2d), 1), -alt)
        wp3d = np.hstack([wp2d[:, :2], down])
        return WaypointTrajectory(wp3d, cruise_speed=float(cfg.get('cruise_speed', 2.0)))
    if t == 'circle':
        return CircleTrajectory(center=cfg.get('center', [0, 0]),
                                radius=float(cfg.get('radius', 10.0)),
                                speed=float(cfg.get('speed', 2.0)),
                                altitude=float(cfg.get('altitude', 5.0)),
                                laps=float(cfg.get('laps', 1.0)))
    raise ValueError(f"unknown trajectory type: {t}")
