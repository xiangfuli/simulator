#!/usr/bin/env bash

project_dir=$(dirname "$(realpath "$0")")/..
current_dir=$project_dir/scripts

BASE_DIR=$project_dir/airsim/examples
for dir in "$BASE_DIR"/*/; do
    folder_name=$(basename "$dir")

    if [ "$folder_name" == "python" ]; then
        continue
    fi

    echo "Building in: $dir"

    cd "$dir" || exit 1

    mkdir -p build
    cd build || exit 1
    cmake ..
    make -j$(nproc)

    cd "$BASE_DIR" || exit 1
done

echo "All builds completed!"