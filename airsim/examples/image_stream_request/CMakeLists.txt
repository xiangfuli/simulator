cmake_minimum_required(VERSION 3.5.0)
project(image_stream_request)

# AirSim Cmake
include("${CMAKE_CURRENT_LIST_DIR}/../../../cmake/airsim.cmake")

## Specify additional locations of header files
include_directories(
  ${AIRSIM_INCLUDES}
)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(PkgConfig REQUIRED)
pkg_search_module(gstreamer REQUIRED IMPORTED_TARGET gstreamer-1.0>=1.4)
pkg_search_module(gstreamer-app REQUIRED IMPORTED_TARGET gstreamer-app-1.0>=1.4)


# add source
add_executable(h264_stream_request
  h264_stream_request.cpp
)

add_executable(mp4_stream_request
  mp4_stream_request.cpp
)

target_link_libraries(h264_stream_request Threads::Threads)
target_link_libraries(h264_stream_request ${AIR_LIB})
target_link_libraries(h264_stream_request ${RPC_LIB})
target_link_libraries(h264_stream_request ${OpenCV_LIBS})
target_link_libraries(h264_stream_request PkgConfig::gstreamer
  PkgConfig::gstreamer-app
)


target_link_libraries(mp4_stream_request Threads::Threads)
target_link_libraries(mp4_stream_request ${AIR_LIB})
target_link_libraries(mp4_stream_request ${RPC_LIB})
target_link_libraries(mp4_stream_request ${OpenCV_LIBS})
target_link_libraries(mp4_stream_request PkgConfig::gstreamer
  PkgConfig::gstreamer-app
)



