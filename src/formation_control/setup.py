from setuptools import find_packages, setup

package_name = 'formation_control'

setup(
    name=package_name,
    version='0.1.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages', ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools', 'numpy'],
    zip_safe=True,
    maintainer='you',
    maintainer_email='you@example.com',
    description='ROS 2 nodes wiring formation_core to PX4.',
    license='BSD-3-Clause',
    entry_points={
        'console_scripts': [
            'drone_interface = formation_control.drone_interface_node:main',
            'coordinator = formation_control.coordinator_node:main',
        ],
    },
)
