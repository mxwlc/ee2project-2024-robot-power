cmake_minimum_required(VERSION 3.28)
project(ee2project_2024_robot_power)
#include_directories(/usr/local/include/opencv4)
#target_link_libraries(/usr/local/include/opencv4)
set(CMAKE_CXX_STANDARD 14)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})


add_executable(ee2project_2024_robot_power
        computer_vision_cpp/open-cv-ctest.cpp)
target_link_libraries(ee2project_2024_robot_power ${OpenCV_LIBS})