Start AirSim Here!
===

This repo is designed for launching the airsim and building your own airsim client examples.

How to include all airsim libs
===

In this repo, all airsim related stuff are installed by `scripts/setup.bash` and `scripts/build.sh`.

And all the required head files and libraries are include in the CMAKE variables `AIRSIM_INCLUDES`, `AIR_LIB` and `RPC_LIB`. To use these two variables, you just need to add `cmake/airsim.cmake` inside your own project like we did in the examples. 
