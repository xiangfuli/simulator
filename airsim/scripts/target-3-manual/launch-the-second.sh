#!/bin/bash
current_directory="$(cd "$(dirname "$0")" && pwd)"
project_root_dir=${current_directory}/../..

PX4_DIR=~/projects/PX4-Autopilot
UNREAL_BLOCK_BIN=~/projects/simulator/libs/CustomEnv/LinuxNoEditor/CustomEnv/Binaries/Linux/CustomEnv

cd $PX4_DIR && make px4_sitl
PX4_SYS_AUTOSTART=10016 ./build/px4_sitl_default/bin/px4 -i 1
