### Before executing this scripts, make sure you have install 
### 1. cmake (>3.10.0) 
### 2. wget
### 3. git
### ... and other build essential tools.

#!/usr/bin/env bash

init_px4=false
ubuntu20=false
for arg in "$@"; do
  case $arg in
    --px4)
      echo "PX4 Will be initialized"
      init_px4=true
      ;;
    --ubuntu20)
      echo "Enable Ubuntu20"
      ubuntu20=true
      ;;
  esac
done

project_dir=$(dirname "$(realpath "$0")")/..
current_dir=$project_dir/scripts

# first install all the dependencies
# prepare for the airsim dependency
mkdir -p $project_dir/libs

# download the airsim library
cd $project_dir/libs
airsim_dir=$project_dir/libs/AirSim
if [ ! -d $airsim_dir ]; then
  git clone https://github.com/xiangfuli/AirSim.git --depth 1
  cd AirSim
  if $ubuntu20; then
    git fetch origin 1.8.1-ubuntu20:1.8.1-ubuntu20
  else
    git fetch origin 1.8.1-ubuntu22:1.8.1-ubuntu22  
  fi
  # set to use gcc instead of clang
  sed -i 's/CC="gcc-8"/CC="gcc"/' build.sh
  sed -i 's/CXX="g++-8"/CXX="g++"/' build.sh
  cd -
fi
if [ -d $airsim_dir ]; then
  cd AirSim
  bash setup.sh --no-full-poly-car
  cd -
fi


if $init_px4; then
  # setup the PX4 environment
  cd $project_dir/libs
  px4_dir=$project_dir/libs/PX4-Autopilot
  if [ ! -d $px4_dir ]; then
    git clone https://github.com/PX4/PX4-Autopilot.git
    cd PX4-Autopilot
    git fetch
    git checkout origin/v1.15.4 -b v1.15.4
    git submodule update --init
  fi
fi
