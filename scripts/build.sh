#!/usr/bin/env bash

project_dir=$(dirname "$(realpath "$0")")/..
current_dir=$project_dir/scripts

# build airsim
cd $project_dir/libs/AirSim/
bash build.sh --gcc
cd -

# build PX4_Autopilot
px4_dir=$project_dir/libs/PX4-Autopilot
if [ -d $px4_dir ]; then
  cd $px4_dir
  DONT_RUN=1 make px4_sitl
  cd -
fi