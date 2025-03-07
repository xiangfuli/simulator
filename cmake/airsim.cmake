set(AIRSIM_ROOT "${CMAKE_CURRENT_LIST_DIR}/../libs/AirSim")
set(AIRLIB_CMAKE_PATH "${AIRSIM_ROOT}/cmake")
LIST(APPEND CMAKE_MODULE_PATH "${AIRLIB_CMAKE_PATH}/cmake-modules") 
INCLUDE("${AIRLIB_CMAKE_PATH}/cmake-modules/CommonSetup.cmake")
IncludeEigen()
find_package(Threads REQUIRED)

set(AIRSIM_INCLUDES
  "${AIRSIM_ROOT}/AirLib/include"
  "${RPC_LIB_INCLUDES}"
  "${AIRSIM_ROOT}/MavLinkCom/include"
  "${AIRSIM_ROOT}/MavLinkCom/common_utils"
)

file(GLOB AIRSIM_STATIC_LIBS "${AIRSIM_ROOT}/AirLib/lib/*/*/**.a")
foreach(LIB ${AIRSIM_STATIC_LIBS})
    if (LIB MATCHES "librpc\\.a$")
        set(RPC_LIB ${LIB})
        message(STATUS "FOUND RPC_LIB: ${RPC_LIB}")
    endif()
    if (LIB MATCHES "libAirLib\\.a$")
        set(AIR_LIB ${LIB})
        message(STATUS "FOUND AIR_LIB: ${AIR_LIB}")
    endif()
endforeach()

set(AIRSIM_LIBRARIES "${RPC_LIB}" "${AIR_LIB}")