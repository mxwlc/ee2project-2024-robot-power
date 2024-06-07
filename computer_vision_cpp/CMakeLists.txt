project(ee2-computer-vision)
SET("OpenCV_DIR" "/usr/share/OpenCV")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

find_package( OpenCV REQUIRED )
#include_directories( ${OpenCV_INCLUDE_DIRS} )

MESSAGE(STATUS "Include dirs ${OpenCV_INCLUDE_DIRS}")
MESSAGE(STATUS "LINK LIBRARIES ${OpenCV_LIBS}")

set(INCLUDE ${PROJECT_SOURCE_DIR}/include/ CACHE FILEPATH "Include")

file(GLOB ALL_FILES
        "${PROJECT_SOURCE_DIR}/src/*.c",
        "${PROJECT_SOURCE_DIR}/src/marker_dict.cpp",
        "${PROJECT_SOURCE_DIR}/include/*.h",
        "${PROJECT_SOURCE_DIR}/include/*.hpp"
)


add_subdirectory(src)
link_directories(include)

add_executable( camera_test ${PROJECT_SOURCE_DIR}/src/webcam_test.cpp ${ALL_FILES} )
add_executable( marker ${PROJECT_SOURCE_DIR}/src/marker_gen.cpp ${ALL_FILES} )
add_executable( detector ${PROJECT_SOURCE_DIR}/src/marker_detection.cpp ${ALL_FILES} )
add_executable( test_memory ${PROJECT_SOURCE_DIR}/src/test_writing.cpp ${ALL_FILES} )

target_link_libraries( camera_test ${OpenCV_LIBS} )
target_link_libraries( marker ${OpenCV_LIBS} )
target_link_libraries( detector ${OpenCV_LIBS} )
target_link_libraries( test_memory ${OpenCV_LIBS} )

target_include_directories( marker PRIVATE ${INCLUDE}/include )
target_include_directories( detector PRIVATE ${INCLUDE}/include )
target_include_directories( camera_test PRIVATE ${INCLUDE}/include )
target_include_directories( test_memory PRIVATE ${INCLUDE/}include )
