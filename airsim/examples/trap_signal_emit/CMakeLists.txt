cmake_minimum_required(VERSION 3.5.0)
project(trap_signal_emit)

# AirSim Cmake
include("${CMAKE_CURRENT_LIST_DIR}/../../../cmake/airsim.cmake")

## Specify additional locations of header files
include_directories(
  ${AIRSIM_INCLUDES}
)

# add source
add_executable(${PROJECT_NAME}
  trap_signal_emit.cpp
)

target_link_libraries(${PROJECT_NAME} Threads::Threads)
target_link_libraries(${PROJECT_NAME} ${AIR_LIB})
target_link_libraries(${PROJECT_NAME} ${RPC_LIB})


