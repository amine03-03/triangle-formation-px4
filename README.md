# Triangle Formation Flight — PX4 · Gazebo · ROS 2

A multi-UAV formation-control stack that flies three quadrotors in an **apex-forward equilateral triangle** along a waypoint path in simulation. The formation is driven by a **virtual-leader** architecture with heading-aligned turns and an optional **displacement-based consensus** term for inter-agent robustness.

<p align="center">
  <!-- PLACEHOLDER: hero shot of the triangle in Gazebo -->
  <img src="docs/media/hero_triangle.png" alt="Triangle formation in Gazebo" width="760"/>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/PX4-v1.17-blue" />
  <img src="https://img.shields.io/badge/ROS_2-Humble-22314E?logo=ros" />
  <img src="https://img.shields.io/badge/Gazebo-Harmonic-orange" />
  <img src="https://img.shields.io/badge/Bridge-uXRCE--DDS-green" />
  <img src="https://img.shields.io/badge/tests-pytest-brightgreen" />
  <img src="https://img.shields.io/badge/license-BSD--3-lightgrey" />
</p>

---

## Contents

- [Overview](#overview)
- [Demo](#demo)
- [Approach and Mathematics](#approach-and-mathematics)
- [System Architecture](#system-architecture)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Configuration](#configuration)
- [Extending the Stack](#extending-the-stack)
- [Coordinate Frames and Troubleshooting](#coordinate-frames-and-troubleshooting)
- [Testing](#testing)
- [Repository Layout](#repository-layout)
- [Roadmap](#roadmap)
- [License](#license)

---

## Overview

The system coordinates three PX4 vehicles through a single **virtual leader** — a phantom reference pose that follows the mission path. No physical drone owns the trajectory, which removes the single point of failure and the error-inheritance problem of naïve leader–follower schemes. Each drone tracks a fixed geometric offset from the leader, and the whole formation shares one heading so the triangle rotates to stay aligned with the direction of travel.

Design highlights:

- **Virtual leader at the centroid** of an apex-forward equilateral triangle, giving a balanced, minimal turn-radius layout.
- **Shared, rate-limited yaw** so heading-aligned corners remain kinematically feasible.
- **Optional consensus term** that corrects inter-drone geometry using only neighbor information; disabled by default, enabled from configuration with no code changes.
- **Plugin architecture** for both trajectory generation and formation-feedback laws, kept in a ROS-free core library that is unit-testable without a simulator.

---

## Demo

<!-- PLACEHOLDER: screen recording of a full run.
     To embed video on GitHub: open an issue, drag your .mp4 into the comment,
     and paste the generated user-attachments URL here. -->

<p align="center">
  <a href="docs/media/formation_run.mp4">
    <img src="docs/media/video_thumbnail.png" alt="Watch the simulation run" width="760"/>
  </a>
</p>

| Assemble phase | Waypoint following |
|:---:|:---:|
| <img src="docs/media/assemble.png" width="380"/> | <img src="docs/media/engage.png" width="380"/> |

<!-- PLACEHOLDER: replace the four images above with your own captures -->

---

## Approach and Mathematics

### Virtual leader and formation offsets

The virtual leader supplies a world-frame position $\mathbf{p}_L \in \mathbb{R}^3$ (NED) and a heading $\psi$. Each drone $i$ tracks a setpoint formed by rotating its **body-frame offset** $\mathbf{o}_i$ into the world frame by the leader yaw:

$$
\mathbf{p}_i^{\text{sp}} = \mathbf{p}_L + R_z(\psi)\,\mathbf{o}_i,
\qquad
\psi_i = \psi .
$$

The NED yaw rotation, with body axes x-forward, y-right, z-down, is

$$
R_z(\psi) =
\begin{bmatrix}
\cos\psi & -\sin\psi & 0 \\
\sin\psi & \cos\psi & 0 \\
0 & 0 & 1
\end{bmatrix}.
$$

### Triangle geometry

For side length $s$, with the virtual leader anchored at the **centroid**, the three body-frame slots are

$$
\mathbf{o}_0 = \left( \frac{s}{\sqrt{3}},\ 0,\ 0 \right) \text{ (apex)}, \qquad
\mathbf{o}_1 = \left( -\frac{s}{2\sqrt{3}},\ \frac{s}{2},\ 0 \right), \qquad
\mathbf{o}_2 = \left( -\frac{s}{2\sqrt{3}},\ -\frac{s}{2},\ 0 \right).
$$

Every slot lies at the circumradius

$$
R = \frac{s}{\sqrt{3}} \approx 0.577\,s ,
$$

which minimizes the worst-case rotational radius and therefore the turn penalty.

### Heading-aligned turns

At a raw waypoint corner the path tangent is discontinuous. Feeding it directly to the yaw command would snap the heading, displacing a drone at radius $R$ by approximately $R\,\Delta\psi$ in a single step. The leader yaw is therefore slewed toward the target heading $\psi^\star$ under a rate limit each control cycle:

$$
\psi_{k+1} = \psi_k + \mathrm{clamp}\!\left( \mathrm{wrap}(\psi^\star - \psi_k),\ -\omega_{\max}\,\Delta t,\ \omega_{\max}\,\Delta t \right).
$$

### Turn-feasibility constraint

When the formation rotates at yaw rate $\omega$ while translating at speed $v$, a drone at radius $R$ reaches a ground speed of up to $v + \omega R$. Keeping this below the vehicle limit couples cruise speed, triangle size, and maximum turn rate:

$$
\omega_{\max} \ \lesssim\ \frac{v_{\max} - v}{R}.
$$

This is the primary knob to reach for if turns feel sluggish or the outer drone saturates.

### Displacement-based consensus (optional)

Layered on top of the leader reference, each drone can additionally correct toward its target geometry relative to its neighbors $\mathcal{N}_i$:

$$
\mathbf{u}_i = -k \sum_{j \in \mathcal{N}_i} \left[ (\mathbf{p}_i - \mathbf{p}_j) - (\mathbf{o}_i^{\,w} - \mathbf{o}_j^{\,w}) \right],
\qquad
\mathbf{o}_i^{\,w} = R_z(\psi)\,\mathbf{o}_i .
$$

The correction is magnitude-clipped and added to the position setpoint. It is enabled by setting `feedback: {type: consensus}` in the configuration.

### Mission state machine

```
INIT  ->  ASSEMBLE  ->  ENGAGE  ->  HOLD  ->  LAND
arm &     fly to        leader     hover     auto
climb     triangle      traverses  at end    land
          slots         waypoints
```

---

## System Architecture

The stack is split into four packages along its two extension points. The core algorithms carry no ROS dependency, so the geometry, frame math, and control laws can be tested in isolation.

| Package | Responsibility |
|---|---|
| `formation_core` | Pure-Python algorithms: trajectories, triangle geometry, frame and yaw math, feedback laws. Unit-tested, no ROS dependency. |
| `formation_interfaces` | Custom message definitions: `LeaderState`, `DroneState`. |
| `formation_control` | ROS 2 (`rclpy`) nodes: `coordinator` (mission and state machine), `drone_interface` (per-drone offboard control), and the `px4_io` bridge (PX4 topics plus frame conversion). |
| `formation_bringup` | Launch files, the central `config/formation.yaml`, and the PX4 spawn script. |

Data flow is distributed rather than centralized: the coordinator owns the mission clock, state machine, and yaw smoothing, and broadcasts a single `LeaderState`. Each `drone_interface` computes its own setpoint from that reference plus the optional consensus term, and runs the PX4 offboard handshake independently. This mirrors a real distributed deployment and keeps the consensus term genuinely local to each agent.

**Extension point 1 — trajectories:** `formation_core/trajectories/` with `registry.py`
**Extension point 2 — feedback:** `formation_core/feedback/` with `registry.py`

---

## Requirements

- Ubuntu 22.04 with ROS 2 **Humble**
- PX4-Autopilot on the **`release/1.17`** branch, built for SITL (`make px4_sitl`), which pulls Gazebo **Harmonic**
- The Micro-XRCE-DDS Agent (`MicroXRCEAgent`) available on `PATH`

> `px4_msgs` must be checked out to `release/1.17` to match the firmware. A version mismatch lets topics connect but deserialize incorrectly.

---

## Installation

```bash
mkdir -p ~/swarm/formation_ws/src && cd ~/swarm/formation_ws/src

# place the four formation_* packages here, then fetch the matching messages:
git clone -b release/1.17 https://github.com/PX4/px4_msgs.git

cd ~/swarm/formation_ws
source /opt/ros/humble/setup.bash
colcon build
source install/setup.bash
```

---

## Usage

**1. Start PX4, Gazebo, and the DDS agent (three instances):**

```bash
PX4_DIR=~/PX4-Autopilot bash src/formation_bringup/scripts/start_px4_instances.sh
ros2 topic list | grep px4_        # expect /px4_1, /px4_2, /px4_3
```

**2. Launch the formation-control layer:**

```bash
source install/setup.bash
ros2 launch formation_bringup formation.launch.py
```

The console reports the active phase as the mission progresses through `INIT`, `ASSEMBLE`, `ENGAGE`, `HOLD`, and `LAND`.

---

## Configuration

All tunable parameters live in `config/formation.yaml`, which the launch file distributes to every node.

```yaml
side: 4.0             # triangle side length (m)
altitude: 5.0         # formation altitude (m)
cruise_speed: 2.0     # virtual-leader speed (m/s)
yaw_rate_max: 0.5     # leader yaw slew limit (rad/s) -> corner feasibility

feedback:             # extension point 2
  type: zero          # 'zero' | 'consensus'
  gain: 0.8
  max_correction: 1.5

trajectory:           # extension point 1
  type: waypoint      # 'waypoint' | 'circle' | custom
  waypoints: [[0, 0], [10, 0], [10, 10], [0, 10], [0, 0]]   # [north, east]
```

---

## Extending the Stack

**Add a trajectory type.** Subclass `TrajectoryBase` (implement `sample(t)` returning a `LeaderState`, plus `duration()`), register it in `trajectories/registry.py`, and select it by name in the configuration. An analytic `CircleTrajectory` is included as a reference implementation.

**Enable or modify consensus feedback.** Set `feedback: {type: consensus, gain: 0.8}` in the configuration. No plumbing changes are required because each drone already publishes and subscribes to peer `DroneState` messages in the default build. To implement a different law, subclass `FormationFeedback` and register it.

---

## Coordinate Frames and Troubleshooting

Each PX4 EKF origin is set to its own vehicle's spawn point, so the three drones do not automatically share a world frame. Positions are reconciled with per-drone spawn offsets:

$$
\mathbf{p}^{\,\text{world}} = \mathbf{p}^{\,\text{local}} + \mathbf{p}^{\,\text{spawn}} .
$$

The `spawn_ned` values in `config/formation.yaml` must match the spawn poses in `start_px4_instances.sh`, using the mapping

$$
\text{world NED } (n, e) \ \longleftrightarrow\ \text{Gazebo ENU } (x = e,\ y = n).
$$

If the formation appears rigidly shifted by a constant vector, this mapping is the first thing to verify.

**Topic names.** Confirm the PX4 topic paths against your build with `ros2 topic list`. Some firmware revisions expose versioned names such as `vehicle_local_position_v1`; the subscription paths in `px4_io.py` must match exactly, or telemetry will not arrive and the coordinator will never leave `ASSEMBLE`.

---

## Testing

The ROS-free core can be validated without a simulator:

```bash
cd src/formation_core && python -m pytest test/ -q
```

The suite covers triangle geometry, the centroid property, the yaw rate limiter, the frame rotation, and waypoint sampling.

---

## Repository Layout

```
formation_ws/
├── README.md
├── docs/media/                 # screenshots and recordings
└── src/
    ├── formation_core/         # algorithms (pure Python)
    ├── formation_interfaces/   # custom messages
    ├── formation_control/      # ROS 2 nodes and PX4 bridge
    └── formation_bringup/      # launch, config, spawn scripts
```

---

## Roadmap

- [ ] Tune the consensus term in closed-loop flight
- [ ] Add a minimum-snap smooth trajectory type
- [ ] Altitude-staggered (non-coplanar) formations
- [ ] Hardware bring-up over a real uXRCE-DDS link

---

## License

Released under the BSD-3-Clause license.
