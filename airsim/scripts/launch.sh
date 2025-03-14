#!/bin/bash
current_directory="$(cd "$(dirname "$0")" && pwd)"
project_root_dir=${current_directory}/../..

PX4_DIR=$1
UNREAL_BLOCK_BIN=$3

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
  CHILD_PIDS=$(pgrep -P $AIRSIM_ROS_ADAPTER_PID)
  echo "Killing AIRSIM_ROS_ADAPTER_PID $AIRSIM_ROS_ADAPTER_PID"
  kill_process_tree $AIRSIM_ROS_ADAPTER_PID
  sleep 0.5

  echo "Killing AIRSIM_ROS_PID $AIRSIM_ROS_PID"
  kill_process_tree $AIRSIM_ROS_PID
  sleep 0.5

  # echo "Killing AIRISM_INIT_PYTHON $AIRISM_INIT_PYTHON"
  # pkill -9 -P $AIRISM_INIT_PYTHON

  echo "Killing AIRSIM_PID $AIRSIM_PID"
  kill_process_tree $AIRSIM_PID
  sleep 0.5

  if [ -n "$AIRSIM_ROS_CORE_PID" ]; then
    echo "Killing AIRSIM_ROS_CORE_PID $AIRSIM_ROS_CORE_PID"
    kill_process_tree $AIRSIM_ROS_CORE_PID
  fi
}

trap cleanup SIGINT

# launch the simulator
$UNREAL_BLOCK_BIN -settings="$project_root_dir/airsim/settings/px4-settings.json" -windowed -vulkan 2>&1 > $current_directory/../logs/airsim.log &
AIRSIM_PID=$!
echo "Airsim sunning on process $AIRSIM_PID"

cd $PX4_DIR && make px4_sitl none_iris
