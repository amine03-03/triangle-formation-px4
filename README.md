<!-- 
  Triangle Formation Flight — PX4 · Gazebo · ROS 2
  Professional README template for GitHub (no macros, no Jinja)
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
\psi_{k+1} = \psi_k + \text{clamp}\!\big(\text{wrap}(\psi^\star - \psi_k),\ -\omega_{\max}\,\Delta t,\ +\omega_{\max}\,\Delta t\big)
$$

> **Note:** `\operatorname` is not supported by GitHub's math renderer. Use `\text{...}` or plain text instead.

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
