import numpy as np
from formation_core.formations.triangle import TriangleFormation
from formation_core.math.frames import rot_z_ned
from formation_core.math.yaw import rate_limit_yaw, wrap_pi


def test_triangle_is_equilateral():
    offs = TriangleFormation(side=4.0).offsets()
    pts = [offs[0], offs[1], offs[2]]
    d01 = np.linalg.norm(pts[0] - pts[1])
    d12 = np.linalg.norm(pts[1] - pts[2])
    d02 = np.linalg.norm(pts[0] - pts[2])
    assert np.allclose([d01, d12, d02], 4.0, atol=1e-9)


def test_centroid_at_origin():
    offs = TriangleFormation(side=4.0).offsets()
    centroid = sum(offs.values()) / 3.0
    assert np.allclose(centroid, 0.0, atol=1e-9)


def test_max_radius_is_circumradius():
    f = TriangleFormation(side=4.0)
    assert abs(f.max_radius() - 4.0 / np.sqrt(3.0)) < 1e-9


def test_rotation_apex_faces_east_at_90deg():
    apex = TriangleFormation(side=4.0).offsets()[0]
    world = rot_z_ned(np.pi / 2.0) @ apex   # facing East
    assert abs(world[1] - 4.0 / np.sqrt(3.0)) < 1e-6  # +East
    assert abs(world[0]) < 1e-6                        # ~0 North


def test_yaw_rate_limit_caps_step():
    out = rate_limit_yaw(0.0, np.pi / 2.0, max_rate=1.0, dt=0.1)
    assert abs(out - 0.1) < 1e-9  # slews +0.1 rad toward +pi/2


def test_waypoint_trajectory_midpoint():
    from formation_core.trajectories.waypoint import WaypointTrajectory
    wp = np.array([[0, 0, -5], [10, 0, -5]], float)
    traj = WaypointTrajectory(wp, cruise_speed=2.0)
    ls = traj.sample(1.0)  # 2 m along +North
    assert np.allclose(ls.position, [2, 0, -5], atol=1e-6)
    assert abs(ls.yaw) < 1e-6  # heading North
