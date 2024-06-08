project(ee2-computer-vision)
SET("OpenCV_DIR" "/usr/share/OpenCV")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

find_package( OpenCV REQUIRED )
#include_directories( ${OpenCV_INCLUDE_DIRS} )

MESSAGE(STATUS "OpenCV_INCLUDE_DIRS :  ${OpenCV_INCLUDE_DIRS}")
MESSAGE(STATUS "OpenCV_LIBS :  ${OpenCV_LIBS}")

set(INCLUDE ${PROJECT_SOURCE_DIR}/include/ CACHE FILEPATH "Include")

file(GLOB ALL_FILES
        "${PROJECT_SOURCE_DIR}/src/*.c",
        "${PROJECT_SOURCE_DIR}/src/*.cpp",
        "${PROJECT_SOURCE_DIR}/include/*.h",
        "${PROJECT_SOURCE_DIR}/include/*.hpp"
)

list(APPEND EXCLUDE_FILES
        "${PROJECT_SOURCE_DIR}/src/marker_gen.cpp",
        "${PROJECT_SOURCE_DIR}/src/marker_detect_camera.cpp",
        "${PROJECT_SOURCE_DIR}/src/test_writing.cpp",
        "${PROJECT_SOURCE_DIR}/src/marker_detect.cpp"
)



list(REMOVE_ITEM ALL_FILES ${EXCLUDE_FILES})

add_subdirectory(src)
link_directories(include)

include_directories( include )

add_custom_target(clear
        COMMAND ${CMAKE_COMMAND} -E echo "Emptying /bin"
        COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_SOURCE_DIR} ./scripts/clear_binaries.sh
        COMMENT "Clears /markers and/bin"
        VERBATIM

)

add_executable( MarkerDetect ${PROJECT_SOURCE_DIR}/src/marker_detect.cpp ${ALL_FILES} )
add_executable( MarkerGen ${PROJECT_SOURCE_DIR}/src/marker_gen.cpp ${ALL_FILES} )
add_executable( ImageDetector ${PROJECT_SOURCE_DIR}/src/marker_detect_camera.cpp ${ALL_FILES} )
add_executable( TestMemory ${PROJECT_SOURCE_DIR}/src/test_writing.cpp ${ALL_FILES} )

target_link_libraries( MarkerDetect ${OpenCV_LIBS} )
target_link_libraries( MarkerGen ${OpenCV_LIBS} )
target_link_libraries( ImageDetector ${OpenCV_LIBS} )
target_link_libraries( TestMemory ${OpenCV_LIBS} )

target_include_directories( MarkerDetect PRIVATE ${INCLUDE}/include )
target_include_directories( MarkerGen PRIVATE ${INCLUDE}/include )
target_include_directories( ImageDetector PRIVATE ${INCLUDE}/include )
target_include_directories( TestMemory PRIVATE ${INCLUDE/}include )
