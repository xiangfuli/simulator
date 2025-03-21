cmake_minimum_required(VERSION 3.5.0)
project(visualization_cameras)

# AirSim Cmake
include("${CMAKE_CURRENT_LIST_DIR}/../../../cmake/airsim.cmake")

find_package(OpenCV REQUIRED)

## Specify additional locations of header files
include_directories(
  ${AIRSIM_INCLUDES}
  ${OpenCV_INCLUDE_DIRS}
)

# add source
add_executable(${PROJECT_NAME}
  visualization_cameras.cpp
)

target_link_libraries(${PROJECT_NAME} Threads::Threads)
target_link_libraries(${PROJECT_NAME} ${AIR_LIB})
target_link_libraries(${PROJECT_NAME} ${RPC_LIB})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

