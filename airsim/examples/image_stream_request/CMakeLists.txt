cmake_minimum_required(VERSION 3.5.0)
project(image_stream_request)

# AirSim Cmake
include("${CMAKE_CURRENT_LIST_DIR}/../../../cmake/airsim.cmake")

## Specify additional locations of header files
include_directories(
  ${AIRSIM_INCLUDES}
)

# add source
add_executable(h264_stream_request
  h264_stream_request.cpp
)

target_link_libraries(h264_stream_request Threads::Threads)
target_link_libraries(h264_stream_request ${AIR_LIB})
target_link_libraries(h264_stream_request ${RPC_LIB})


