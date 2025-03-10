#!/usr/bin/env bash

project_dir=$(dirname "$(realpath "$0")")/..
current_dir=$project_dir/scripts

cd airsim/examples/image_request

rm -rf build && mkdir build && cd build
cmake ..
cmake --build .