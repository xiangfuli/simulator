Start AirSim Here!
===

This repo is designed for launching the airsim and building your own airsim client examples.

How to include all airsim libs
===

In this repo, all airsim related stuff are installed by `scripts/setup.ash` and `scripts/build.sh`.

And all the required head files and libraries are include in the CMAKE variables `AIRSIM_INCLUDES` and `AIRSIM_LIBS`. To use these two variables, you just need to add `cmake/AirSim.cmake` inside your own project like we did in the examples. 