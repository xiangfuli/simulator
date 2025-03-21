#!/bin/bash
current_directory="$(cd "$(dirname "$0")" && pwd)"
project_root_dir=${current_directory}/../..

PX4_DIR=~/projects/PX4-Autopilot
UNREAL_BLOCK_BIN=~/projects/simulator/libs/CustomEnv/LinuxNoEditor/CustomEnv/Binaries/Linux/CustomEnv

kill_process_tree() {
  PARENT_PID=$1
  CHILD_PIDS=$(pgrep -P $PARENT_PID)

  for CHILD in $CHILD_PIDS; do
    kill_process_tree $CHILD
  done

  echo "Killing process $PARENT_PID"
  kill -9 $PARENT_PID
}

cleanup() {
  echo "Killing AIRSIM_PID $AIRSIM_PID"
  kill_process_tree $AIRSIM_PID
  sleep 0.5
}

trap cleanup SIGINT

# launch the simulator
mkdir -p $current_directory/../logs/
$UNREAL_BLOCK_BIN -settings="$current_directory/airsim-settings.json" -windowed -vulkan 2>&1 > $current_directory/../logs/airsim.log &
AIRSIM_PID=$!
echo "Airsim running on process $AIRSIM_PID"





cd $PX4_DIR && make px4_sitl none_iris
