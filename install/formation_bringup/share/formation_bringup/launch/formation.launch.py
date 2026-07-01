"""Bring up the formation control layer (NOT PX4/Gazebo).

Run PX4 + the micro-XRCE-DDS agent first (see scripts/start_px4_instances.sh
and the README), then launch this. Reads config/formation.yaml as the single
source of truth and distributes params to the coordinator and each drone.
"""
import os
import yaml
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    share = get_package_share_directory('formation_bringup')
    cfg_path = os.path.join(share, 'config', 'formation.yaml')
    with open(cfg_path, 'r') as f:
        cfg = yaml.safe_load(f)

    drones = cfg['drones']
    n = len(drones)
    fb = cfg.get('feedback', {'type': 'zero'})
    traj = cfg['trajectory']

    # flatten waypoints [[n,e],...] -> [n,e,n,e,...] for the coordinator param
    wp_flat = [float(v) for pair in traj['waypoints'] for v in pair]

    nodes = []

    nodes.append(Node(
        package='formation_control', executable='coordinator', name='coordinator',
        output='screen',
        parameters=[{
            'n_drones': n,
            'side': cfg['side'],
            'altitude': cfg['altitude'],
            'control_rate': cfg['control_rate'],
            'yaw_rate_max': cfg['yaw_rate_max'],
            'trajectory_type': traj['type'],
            'cruise_speed': cfg['cruise_speed'],
            'waypoints_flat': wp_flat,
        }],
    ))

    for d in drones:
        nodes.append(Node(
            package='formation_control', executable='drone_interface',
            name=f'drone_interface_{d["index"]}', output='screen',
            parameters=[{
                'index': d['index'],
                'namespace': d['namespace'],
                'sys_id': d['sys_id'],
                'spawn_offset_ned': [float(x) for x in d['spawn_ned']],
                'side': cfg['side'],
                'altitude': cfg['altitude'],
                'control_rate': cfg['control_rate'],
                'feedback_type': fb.get('type', 'zero'),
                'feedback_gain': float(fb.get('gain', 0.8)),
                'feedback_max_correction': float(fb.get('max_correction', 1.5)),
            }],
        ))

    return LaunchDescription(nodes)
