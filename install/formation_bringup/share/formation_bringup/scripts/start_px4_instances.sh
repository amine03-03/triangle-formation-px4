#!/usr/bin/env bash
# Spawn 3 PX4 SITL instances (Gazebo Harmonic) + the uXRCE-DDS agent.
#
# Frame contract (MUST match config/formation.yaml spawn_ned):
#   Gazebo model pose is world ENU "x_east,y_north,z_up,...".
#   world NED (north, east) -> gz pose (x=east, y=north).
#     px4_1: NED(0, 0)   -> gz "0,0"     spawn_ned [0, 0, 0]
#     px4_2: NED(0,+3)   -> gz "3,0"     spawn_ned [0, 3, 0]   (3 m East)
#     px4_3: NED(0,-3)   -> gz "-3,0"    spawn_ned [0,-3, 0]   (3 m West)
#
# Instances 1..3 => DDS namespaces px4_1..px4_3, MAV_SYS_ID 2..4.
# (Starting at 1, not 0, avoids the bare-/fmu no-namespace special case.)
#
# Usage:  PX4_DIR=~/PX4-Autopilot ./start_px4_instances.sh
set -e
: "${PX4_DIR:?set PX4_DIR to your PX4-Autopilot checkout (release/1.17)}"
AIRFRAME="${PX4_SYS_AUTOSTART:-4001}"     # 4001 = gz_x500 quad
MODEL="${PX4_SIM_MODEL:-gz_x500}"
BIN="$PX4_DIR/build/px4_sitl_default/bin/px4"

# 1) uXRCE-DDS agent (single agent, all clients connect over UDP 8888)
if ! pgrep -x MicroXRCEAgent >/dev/null; then
  echo "[agent] starting MicroXRCEAgent udp4 -p 8888"
  MicroXRCEAgent udp4 -p 8888 >/tmp/xrce_agent.log 2>&1 &
  sleep 1
fi

start_instance () {
  local i=$1 pose=$2
  echo "[px4] instance $i  gz_pose=$pose  ns=px4_$i  sys_id=$((i+1))"
  PX4_SYS_AUTOSTART=$AIRFRAME \
  PX4_SIM_MODEL=$MODEL \
  PX4_GZ_MODEL_POSE="$pose" \
  PX4_UXRCE_DDS_NS="px4_$i" \
    "$BIN" -i "$i" >/tmp/px4_$i.log 2>&1 &
  sleep 4   # let the first instance bring up the Gazebo server before the rest
}

cd "$PX4_DIR"
start_instance 1 "0,0"     # px4_1  (NED 0,0)
start_instance 2 "3,0"     # px4_2  (NED 0,+3, East)
start_instance 3 "-3,0"    # px4_3  (NED 0,-3, West)

echo "All instances launched. Logs in /tmp/px4_<i>.log and /tmp/xrce_agent.log"
echo "Check topics:  ros2 topic list | grep px4_"
wait
