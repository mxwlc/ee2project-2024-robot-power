project(ee2-computer-vision)
SET("OpenCV_DIR" "/usr/share/OpenCV")
MESSAGE( STATUS "=====================================================" )
MESSAGE( STATUS "Setting Directories" )
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
MESSAGE( STATUS "Setting Directories - done" )
MESSAGE( STATUS "=====================================================" )
find_package(OpenCV REQUIRED)
#include_directories( ${OpenCV_INCLUDE_DIRS} )
MESSAGE(STATUS "=====================================================")
MESSAGE(STATUS "Configuring Libraries")
MESSAGE(STATUS "OpenCV_INCLUDE_DIRS :  ${OpenCV_INCLUDE_DIRS}")
MESSAGE(STATUS "OpenCV_LIBS :  ${OpenCV_LIBS}")
MESSAGE(STATUS "Configuring Libraries - done")
MESSAGE(STATUS "=====================================================")


MESSAGE( STATUS "=====================================================" )
MESSAGE( STATUS "Setting filepaths" )
set(INCLUDE ${PROJECT_SOURCE_DIR}/include/ CACHE FILEPATH "Include")
file(GLOB ALL_FILES
        "${PROJECT_SOURCE_DIR}/src/*.c" ,
        "${PROJECT_SOURCE_DIR}/src/*.cpp" ,
        "${PROJECT_SOURCE_DIR}/include/*.h" ,
        "${PROJECT_SOURCE_DIR}/include/*.hpp"
)

list(APPEND EXCLUDE_FILES
        "${PROJECT_SOURCE_DIR}/src/marker_gen.cpp" ,
        "${PROJECT_SOURCE_DIR}/src/marker_detect_image.cpp" ,
        "${PROJECT_SOURCE_DIR}/src/test_writing.cpp" ,
        "${PROJECT_SOURCE_DIR}/src/marker_detect.cpp"
)
MESSAGE( STATUS "Setting Filepaths - done" )
MESSAGE( STATUS "=====================================================" )

list(REMOVE_ITEM ALL_FILES ${EXCLUDE_FILES})

add_subdirectory(src)
link_directories(include)

include_directories(include)

MESSAGE( STATUS "=====================================================" )
MESSAGE( STATUS "Configuring Custom Targets" )
add_custom_target(clear
        COMMAND ${CMAKE_COMMAND} -E echo "-----------------------------------"
        COMMAND ${CMAKE_COMMAND} -E echo "Emptying /bin"
        COMMAND ${CMAKE_COMMAND} -E echo "-----------------------------------"
        COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_SOURCE_DIR} ./scripts/clear_binaries.sh
        COMMENT "Clears /bin"
        COMMAND ${CMAKE_COMMAND} -E echo "-----------------------------------"
        COMMAND ${CMAKE_COMMAND} -E echo "Emptying /markers"
        COMMAND ${CMAKE_COMMAND} -E echo "-----------------------------------"
        COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_SOURCE_DIR} ./scripts/clear_markers.sh
        COMMENT "Clears /markers"
        VERBATIM

)

add_custom_target(appendix
        COMMAND ${CMAKE_COMMAND} -E echo "-----------------------------------"
        COMMAND ${CMAKE_COMMAND} -E echo "Sending source code to appendix.txt"
        COMMAND ${CMAKE_COMMAND} -E echo "-----------------------------------"
        COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_SOURCE_DIR} ./scripts/appendix.sh
        COMMENT "Generates an Appendix containing all code."
        VERBATIM

)

add_custom_target(convert
        COMMAND ${CMAKE_COMMAND} -E echo "-----------------------------------"
        COMMAND ${CMAKE_COMMAND} -E echo "Converts Markers from bmp to jpg"
        COMMAND ${CMAKE_COMMAND} -E echo "-----------------------------------"
        COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_SOURCE_DIR} ./scripts/convert_markers.sh
        COMMENT "Converts markers from .bmp to .jpg"
        VERBATIM

)

add_custom_target( clear_markers
COMMAND ${CMAKE_COMMAND} -E echo "-----------------------------------"
COMMAND ${CMAKE_COMMAND} -E echo "Emptying /markers"
COMMAND ${CMAKE_COMMAND} -E echo "-----------------------------------"
COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_SOURCE_DIR} ./scripts/clear_markers.sh
COMMENT "Clears /markers"
VERBATIM
)

add_custom_target( clear_bin
        COMMAND ${CMAKE_COMMAND} -E echo "-----------------------------------"
        COMMAND ${CMAKE_COMMAND} -E echo "Emptying /bin"
        COMMAND ${CMAKE_COMMAND} -E echo "-----------------------------------"
        COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_SOURCE_DIR} ./scripts/clear_binaries.sh
        COMMENT "Clears /bin"
        VERBATIM
)



MESSAGE( STATUS "Configuring Custom Targets - done" )
MESSAGE( STATUS "=====================================================" )

MESSAGE( STATUS "=====================================================" )
MESSAGE( STATUS "Configuring Executables" )
add_executable(MarkerDetect ${PROJECT_SOURCE_DIR}/src/marker_detect.cpp ${ALL_FILES})
add_executable(MarkerGen ${PROJECT_SOURCE_DIR}/src/marker_gen.cpp ${ALL_FILES})
add_executable(ImageDetector ${PROJECT_SOURCE_DIR}/src/marker_detect_image.cpp ${ALL_FILES})
add_executable(Test ${PROJECT_SOURCE_DIR}/src/test_writing.cpp ${ALL_FILES})
add_executable(RPEyes ${PROJECT_SOURCE_DIR}/src/marker_detect_rpi.cpp ${ALL_FILES})

target_link_libraries(MarkerDetect ${OpenCV_LIBS})
target_link_libraries(MarkerGen ${OpenCV_LIBS})
target_link_libraries(ImageDetector ${OpenCV_LIBS})
target_link_libraries(Test ${OpenCV_LIBS})
target_link_libraries(RPEyes ${OpenCV_LIBS})

target_include_directories(MarkerDetect PRIVATE ${INCLUDE}/include)
target_include_directories(MarkerGen PRIVATE ${INCLUDE}/include)
target_include_directories(ImageDetector PRIVATE ${INCLUDE}/include)
target_include_directories(Test PRIVATE ${INCLUDE/}include)
target_include_directories(RPEyes PRIVATE ${INCLUDE/}include)
MESSAGE( STATUS "Configuring Executables - done" )
MESSAGE( STATUS "=====================================================" )
