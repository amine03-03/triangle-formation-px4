# 🛸 Triangle Formation Flight — PX4 · Gazebo · ROS 2

> Three drones hold an **apex-forward equilateral triangle** and follow a waypoint path in simulation, using a **virtual-leader** architecture with an optional **consensus** term for inter-drone robustness.

<p align="center">
  <!-- 📸 PLACEHOLDER: hero shot of the triangle in Gazebo -->
  <img src="docs/media/hero_triangle.png" alt="Triangle formation in Gazebo" width="720"/>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/PX4-v1.17-blue" />
  <img src="https://img.shields.io/badge/ROS_2-Humble-22314E?logo=ros" />
  <img src="https://img.shields.io/badge/Gazebo-Harmonic-orange" />
  <img src="https://img.shields.io/badge/Bridge-uXRCE--DDS-green" />
  <img src="https://img.shields.io/badge/license-BSD--3-lightgrey" />
</p>

---

## 🎬 Demo

<!-- 🎥 PLACEHOLDER: drop your screen recording here.
     Tip: drag an .mp4 into a GitHub issue/PR comment, then paste the generated
     user-images URL below — GitHub will render it inline as a player. -->

<p align="center">
  <a href="https://github.com/user-attachments/assets/305f10ad-7c2a-4d36-b334-47bf534d025d">
    <img src="docs/media/video_thumbnail.png" alt="Watch the simulation run" width="720"/>
  </a>
</p>

| Assemble phase | Waypoint following |
|:---:|:---:|
| ![assemble](docs/media/assemble.png) | ![engage](docs/media/engage.png) |
<!-- 📸 PLACEHOLDER: swap in your own screenshots of ASSEMBLE and ENGAGE -->

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
\sin\psi & \phantom{-}\cos\psi & 0\\
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
\psi_{k+1} = \psi_k + \operatorname{clamp}\!\big(\operatorname{wrap}(\psi^\star - \psi_k),\ -\omega_{\max}\,\Delta t,\ +\omega_{\max}\,\Delta t\big)
$$

### Turn-feasibility constraint

While rotating at yaw rate $\omega$ and translating at speed $v$, a drone at radius $R$ reaches ground speed up to $v + \omega R$. Keeping that under the vehicle limit gives the coupling between speed, triangle size, and how sharply the formation may turn:

$$
\boxed{\ \omega_{\max} \ \lesssim\ \frac{v_{\max} - v}{R}\ }
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
INIT ──▶ ASSEMBLE ──▶ ENGAGE ──▶ HOLD ──▶ LAND
 arm     fly to        leader     hover    auto
 climb   triangle      traverses  at end   land
         slots         waypoints
```

---

## 🗂️ Architecture

Four packages, split along the two extension points:

| Package | Role |
|---|---|
| `formation_core` 🧮 | **Pure Python, zero ROS deps** — trajectories, triangle geometry, frame/yaw math, feedback laws. Unit-tested. |
| `formation_interfaces` 📨 | Custom messages: `LeaderState`, `DroneState`. |
| `formation_control` 🔌 | `rclpy` nodes: `coordinator` (mission), `drone_interface` (per-drone offboard), `px4_io` (PX4 bridge + frames). |
| `formation_bringup` 🚀 | Launch files, `config/formation.yaml` (single source of truth), PX4 spawn script. |

**Extension point 1 — trajectories:** `formation_core/trajectories/` + `registry.py`
**Extension point 2 — consensus:** `formation_core/feedback/` + `registry.py`

The key design choice: `formation_core` has **no ROS dependency**, so the algorithms are testable in seconds without a running sim.

---

## ⚙️ Requirements

- 🐧 Ubuntu 22.04 · ROS 2 **Humble**
- ✈️ PX4-Autopilot on **release/1.17**, built for SITL (`make px4_sitl`) — pulls Gazebo **Harmonic**
- 🌉 `MicroXRCEAgent` on your `PATH`

> ⚠️ `px4_msgs` **must** be checked out to `release/1.17` to match the firmware, or messages deserialize to garbage.

---

## 🏗️ Build

```bash
mkdir -p ~/swarm/formation_ws/src && cd ~/swarm/formation_ws/src
# copy the four formation_* packages here, then:
git clone -b release/1.17 https://github.com/PX4/px4_msgs.git
cd ~/swarm/formation_ws
source /opt/ros/humble/setup.bash
colcon build
source install/setup.bash
```

## ▶️ Run

```bash
# 1) PX4 (x3) + Gazebo + DDS agent
PX4_DIR=~/PX4-Autopilot bash src/formation_bringup/scripts/start_px4_instances.sh
ros2 topic list | grep px4_          # sanity: /px4_1 /px4_2 /px4_3

# 2) formation control layer
ros2 launch formation_bringup formation.launch.py
```

Watch the console step through `INIT → ASSEMBLE → ENGAGE → HOLD → LAND`.

---

## 🎛️ Tuning & extending — all in `config/formation.yaml`

```yaml
side: 4.0            # triangle side (m)
cruise_speed: 2.0    # leader speed (m/s)
yaw_rate_max: 0.5    # corner feasibility (rad/s)

feedback:            # 🔁 extension point 2
  type: zero         # -> 'consensus' to enable relative-position term

trajectory:          # 🧭 extension point 1
  type: waypoint     # -> 'circle', or add your own in registry.py
  waypoints: [[0,0],[10,0],[10,10],[0,10],[0,0]]   # [north, east]
```

---

## 🧭 Frame contract (read if the formation looks shifted)

Each PX4 EKF origin sits at **that vehicle's own spawn point**, so positions are made comparable via `spawn_ned` offsets: `world = local + spawn_offset`. The `spawn_ned` values in `formation.yaml` **must** match the Gazebo poses in `start_px4_instances.sh`:

$$\text{world NED } (n, e) \ \longleftrightarrow\ \text{Gazebo ENU } (x{=}e,\ y{=}n)$$

> 💡 Also match the exact PX4 topic names to your build via `ros2 topic list` — some firmware versions expose versioned names like `vehicle_local_position_v1`.

---

## 🧪 Test the core (no sim needed)

```bash
cd src/formation_core && python -m pytest test/ -q
```

---

## 📁 Repo layout

```
formation_ws/
├── README.md
├── docs/media/               # 📸 screenshots + 🎥 recordings go here
└── src/
    ├── formation_core/        # 🧮 algorithms (pure python)
    ├── formation_interfaces/  # 📨 msgs
    ├── formation_control/     # 🔌 ros 2 nodes
    └── formation_bringup/     # 🚀 launch + config + scripts
```

---

## 🗺️ Roadmap

- [ ] Enable & tune the consensus term in flight
- [ ] Add a min-snap / smooth trajectory type
- [ ] Altitude-staggered formations
- [ ] Hardware bringup (real uXRCE-DDS link)

---

## 📜 License

BSD-3-Clause.
