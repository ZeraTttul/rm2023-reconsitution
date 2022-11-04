cmake_minimum_required(VERSION 3.23)
project(rm2023_reconsitution)

set(CMAKE_CXX_STANDARD 20)

# OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(rm2023_reconsitution
        src/main.cpp
        # include/strategy.h
        src/video_cap.cpp
        define/define.h
        # src/strategy.cpp
        include/video_cap.h
        src/armor_detector.cpp
        include/armor_detector.h
        include/rgb.h
        src/rgb.cpp
        include/SolvePnP.h
        src/SolvePnP.cpp
        src/find_armor_factory.cpp
        include/find_armor_factory.h
        include/kalman.h
        src/kalman.cpp
        include/armor_tracker.h
        src/armor_tracker.cpp)

target_link_libraries(rm2023_reconsitution ${OpenCV_LIBS})
