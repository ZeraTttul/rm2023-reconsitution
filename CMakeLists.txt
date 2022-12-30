cmake_minimum_required(VERSION 3.4)
project(rm2023_reconsitution)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_BUILD_TYPE RELEASE)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPATH=\"\\\"${PROJECT_SOURCE_DIR}\\\"\"")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D${CMAKE_SYSTEM_NAME}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
SET(BIN_NAME "run")

# 定义相机SDK工具包目录
set(HIKVISION_CAMERA_SDK_DIR "${PROJECT_SOURCE_DIR}/hikvision")
# OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
LINK_DIRECTORIES(/opt/MVS/lib/armhf)

# 指定相机库目录
INCLUDE_DIRECTORIES(${HIKVISION_CAMERA_SDK_DIR}/include)
INCLUDE_DIRECTORIES(${HIKVISION_CAMERA_SDK_DIR}/src)

FILE(GLOB_RECURSE INCLUDE_FILES "${PROJECT_SOURCE_DIR}/include/*.h")
FILE(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(${BIN_NAME} ${INCLUDE_FILES} 
                           ${SRC_FILES} 
                           camera/hikvision/tool/calibration/calibrations.cpp 
                           camera/hikvision/tool/calibration/calibrations.h 
                           camera/hikvision/tool/debug/camera_parms_debug.cpp 
                           camera/hikvision/tool/debug/camera_parms_debug.h)

target_link_libraries(${BIN_NAME} ${OpenCV_LIBS})
TARGET_LINK_LIBRARIES(${BIN_NAME} MvCameraControl)
