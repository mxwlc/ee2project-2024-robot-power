cmake_minimum_required(VERSION 3.28)
project(ee2-computer-vision)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(INCLUDE ${PROJECT_SOURCE_DIR}/include/ CACHE FILEPATH "Include")


add_subdirectory(src)
link_directories(include)
add_executable(camera_test src/webcam_test.cpp src/marker_dict.cpp)
add_executable(marker src/marker_gen.cpp src/marker_dict.cpp)
add_executable(detector src/marker_detection.cpp src/marker_dict.cpp)
add_executable(map_test src/map_test.cpp)
target_link_libraries( camera_test ${OpenCV_LIBS})
target_link_libraries( marker ${OpenCV_LIBS})
target_link_libraries( detector ${OpenCV_LIBS} )
target_include_directories(marker PRIVATE ${INCLUDE}/include)
target_include_directories(detector PRIVATE ${INCLUDE}/include)
