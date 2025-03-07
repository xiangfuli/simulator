### Before executing this scripts, make sure you have install 
### 1. cmake (>3.10.0) 
### 2. wget
### 3. git
### ... and other build essential tools.

#!/usr/bin/env bash

project_dir=$(dirname "$(realpath "$0")")/..
current_dir=$project_dir/scripts

# first install all the dependencies
# prepare for the airsim dependency
mkdir -p $project_dir/libs

# download the airsim library
cd $project_dir/libs
airsim_dir=$project_dir/libs/AirSim
if [ ! -d $airsim_dir ]; then
  git clone https://github.com/microsoft/AirSim.git
  cd AirSim
  git fetch
  git checkout origin/v1.8.1 -b v1.8.1

  # set to use gcc instead of clang
  sed -i 's/CC="gcc-8"/CC="gcc"/' build.sh
  sed -i 's/CXX="g++-8"/CXX="g++"/' build.sh

  bash setup.sh
  cd -
fi


# setup the PX4 environment
cd $project_dir/libs
px4_dir=$project_dir/libs/PX4-Autopilot
if [ ! -d $airsim_dir ]; then
  git clone https://github.com/PX4/PX4-Autopilot.git
  cd PX4-Autopilot
  git fetch
  git checkout origin/v1.15.4 -b v1.15.4
fi
