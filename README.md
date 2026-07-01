<!--
  Triangle Formation Flight — PX4 · Gazebo · ROS 2
  Professional README for GitHub (math uses only allowlisted macros)
-->

<h1 align="center">🛸 Triangle Formation Flight</h1>

<p align="center">
  <strong>PX4 · Gazebo · ROS 2</strong><br>
  <em>Virtual-leader formation control with consensus robustness</em>
</p>

<p align="center">
  <a href="https://px4.io/">
    <img src="https://img.shields.io/badge/PX4-v1.17-blue?style=flat-square&logo=px4" alt="PX4 v1.17">
  </a>
  <a href="https://docs.ros.org/en/humble/">
    <img src="https://img.shields.io/badge/ROS_2-Humble-22314E?style=flat-square&logo=ros" alt="ROS 2 Humble">
  </a>
  <a href="https://gazebosim.org/">
    <img src="https://img.shields.io/badge/Gazebo-Harmonic-orange?style=flat-square&logo=gazebo" alt="Gazebo Harmonic">
  </a>
  <a href="https://docs.px4.io/main/en/middleware/uxrce_dds.html">
    <img src="https://img.shields.io/badge/Bridge-uXRCE--DDS-green?style=flat-square" alt="uXRCE-DDS">
  </a>
  <a href="LICENSE">
    <img src="https://img.shields.io/badge/license-BSD--3--Clause-lightgrey?style=flat-square" alt="BSD-3-Clause">
  </a>
</p>

<p align="center">
  <img src="docs/media/Screenshot%20from%202026-07-01%2022-40-27.png" alt="Triangle formation in Gazebo" width="720"/>
</p>

---

## 🎬 Demo

<p align="center">
  <a href="https://github.com/user-attachments/assets/305f10ad-7c2a-4d36-b334-47bf534d025d">
    <img src="docs/media/video_thumbnail.png" alt="Watch the simulation run" width="720"/>
  </a>
</p>

| Assemble phase | Waypoint following |
|:---:|:---:|
| <img src="docs/media/Screenshot%20from%202026-07-01%2022-40-27.png" alt="assemble" width="360"> | <img src="docs/media/Screenshot%20from%202026-07-01%2022-40-46.png" alt="engage" width="360"> |

---

## ✨ What it does

- 🎯 A **virtual leader** (a phantom setpoint, no physical drone) tracks the waypoint path.
- 📐 Three drones sit at fixed **offsets** from the leader, forming an equilateral triangle with the apex pointing along the direction of travel.
- 🧭 The whole formation shares one **yaw** and rotates to stay aligned with the path (heading-aligned).
- 🔁 An optional **consensus / relative-position** term corrects inter-drone geometry — off by default, one line to enable.
- 🧱 Built to extend: new **trajectory types** and new **feedback laws** are drop-in plugins selected from config.

---

## 🧠 Approach & Math

### Virtual leader + formation offsets

The leader provides a world-frame pose $\mathbf{p}_L \in \mathbb{R}^3$ (NED) and heading $\psi$.
Each drone $i$ tracks a setpoint obtained by rotating its **body-frame offset** $\mathbf{o}_i$ into the world by the leader yaw:

$$
\mathbf{p}_i^{\text{sp}} = \mathbf{p}_L + R_z(\psi)\,\mathbf{o}_i,
\qquad
\psi_i = \psi
$$

with the NED yaw rotation (body: x-forward, y-right, z-down)

$$
R_z(\psi) =
\begin{bmatrix}
\cos\psi & -\sin\psi & 0\\
\sin\psi & \cos\psi & 0\\
0 & 0 & 1
\end{bmatrix}.
$$

### Triangle geometry (apex forward, leader at centroid)

For side length $s$, with the virtual leader anchored at the **centroid**, the three slots are

$$
\mathbf{o}_0 = \Big(\tfrac{s}{\sqrt{3}},\ 0,\ 0\Big)\ \text{(apex)},\quad
\mathbf{o}_1 = \Big(-\tfrac{s}{2\sqrt{3}},\ +\tfrac{s}{2},\ 0\Big),\quad
\mathbf{o}_2 = \Big(-\tfrac{s}{2\sqrt{3}},\ -\tfrac{s}{2},\ 0\Big).
$$

All three sit at the circumradius $R = \dfrac{s}{\sqrt{3}} \approx 0.577\,s$ from the leader — a balanced layout that minimizes the worst-case turn load.

### Heading-aligned turns need a smooth yaw ⚠️

At a raw waypoint corner the path tangent is discontinuous, so the commanded yaw would **snap**, teleporting a drone at radius $R$ by $\sim R\,\Delta\psi$. The leader yaw is therefore **rate-limited** each control step:

$$
\psi_{k+1} = \psi_k + \text{clamp}\!\big(\text{wrap}(\psi^\star - \psi_k),\ -\omega_{\max}\,\Delta t,\ +\omega_{\max}\,\Delta t\big)
$$

### Turn-feasibility constraint

While rotating at yaw rate $\omega$ and translating at speed $v$, a drone at radius $R$ reaches ground speed up to $v + \omega R$. Keeping that under the vehicle limit gives the coupling between speed, triangle size, and how sharply the formation may turn:

$$
\omega_{\max} \ \lesssim\ \frac{v_{\max} - v}{R}
$$

### Optional: displacement-based consensus

Layered on top of the leader reference, each drone can correct toward its desired geometry relative to neighbors:

$$
\mathbf{u}_i = -k \sum_{j \in \mathcal{N}_i} \Big[(\mathbf{p}_i - \mathbf{p}_j) - (\mathbf{o}_i^{\,w} - \mathbf{o}_j^{\,w})\Big],
\qquad
\mathbf{o}_i^{\,w} = R_z(\psi)\,\mathbf{o}_i
$$

The output is clipped to a small magnitude and added to the position setpoint. Enable with `feedback: {type: consensus}` in the config.

### Mission state machine

```
INIT  ->  ASSEMBLE  ->  ENGAGE  ->  HOLD  ->  LAND
arm &     fly to        leader     hover     auto
climb     triangle      traverses  at end    land
          slots         waypoints
```

---

## 🗂️ Architecture

Four packages, split along the two extension points. The core algorithms carry no ROS dependency, so geometry, frame math, and control laws are testable without a simulator.

| Package | Responsibility |
|---|---|
| `formation_core` | Pure-Python algorithms: trajectories, triangle geometry, frame/yaw math, feedback laws. Unit-tested, no ROS deps. |
| `formation_interfaces` | Custom messages: `LeaderState`, `DroneState`. |
| `formation_control` | ROS 2 (`rclpy`) nodes: `coordinator` (mission + state machine), `drone_interface` (per-drone offboard), and the `px4_io` bridge (PX4 topics + frame conversion). |
| `formation_bringup` | Launch files, the central `config/formation.yaml`, and the PX4 spawn script. |

Data flow is distributed: the coordinator owns the mission clock, state machine, and yaw smoothing, and broadcasts a single `LeaderState`. Each `drone_interface` computes its own setpoint from that reference plus the optional consensus term, and runs the PX4 offboard handshake independently.

**Extension point 1 — trajectories:** `formation_core/trajectories/` + `registry.py`
**Extension point 2 — feedback:** `formation_core/feedback/` + `registry.py`

---

## ⚙️ Requirements

- Ubuntu 22.04 with ROS 2 **Humble**
- PX4-Autopilot on the **`release/1.17`** branch, built for SITL (`make px4_sitl`) — pulls Gazebo **Harmonic**
- The Micro-XRCE-DDS Agent (`MicroXRCEAgent`) on your `PATH`

> `px4_msgs` **must** be checked out to `release/1.17` to match the firmware, or messages deserialize incorrectly.

---

## 🏗️ Build

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

## ▶️ Usage

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

The console reports the phase as the mission runs: `INIT → ASSEMBLE → ENGAGE → HOLD → LAND`.

---

## 🎛️ Configuration

All tunables live in `config/formation.yaml`, distributed to every node by the launch file.

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

## 🧩 Extending the stack

**Add a trajectory type.** Subclass `TrajectoryBase` (implement `sample(t)` returning a `LeaderState`, plus `duration()`), register it in `trajectories/registry.py`, and select it by name in the config. An analytic `CircleTrajectory` ships as a reference.

**Enable or modify consensus feedback.** Set `feedback: {type: consensus, gain: 0.8}`. No plumbing changes are needed because each drone already publishes and subscribes to peer `DroneState` messages. For a different law, subclass `FormationFeedback` and register it.

---

## 🧭 Coordinate frames & troubleshooting

Each PX4 EKF origin is set to its own vehicle's spawn point, so the drones do not share a world frame automatically. Positions are reconciled with per-drone spawn offsets:

$$
\mathbf{p}^{\,\text{world}} = \mathbf{p}^{\,\text{local}} + \mathbf{p}^{\,\text{spawn}}
$$

The `spawn_ned` values in `config/formation.yaml` must match the spawn poses in `start_px4_instances.sh`, using

$$
\text{world NED } (n, e) \ \longleftrightarrow\ \text{Gazebo ENU } (x = e,\ y = n)
$$

If the formation looks rigidly shifted by a constant vector, verify this mapping first.

> **Topic names:** confirm the PX4 paths against your build with `ros2 topic list`. Some firmware revisions expose versioned names such as `vehicle_local_position_v1`; the subscription paths in `px4_io.py` must match exactly, or telemetry never arrives and the run stalls in `ASSEMBLE`.

---

## 🧪 Testing

The ROS-free core is validated without a simulator:

```bash
cd src/formation_core && python -m pytest test/ -q
```

Covers triangle geometry, the centroid property, the yaw rate limiter, the frame rotation, and waypoint sampling.

---

## 📁 Repository layout

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

## 🗺️ Roadmap

- [ ] Tune the consensus term in closed-loop flight
- [ ] Add a minimum-snap smooth trajectory type
- [ ] Altitude-staggered (non-coplanar) formations
- [ ] Hardware bring-up over a real uXRCE-DDS link

---

## 📜 License

Released under the BSD-3-Clause license.
