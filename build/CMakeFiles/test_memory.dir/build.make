# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/build

# Include any dependencies generated for this target.
include CMakeFiles/test_memory.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test_memory.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test_memory.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_memory.dir/flags.make

CMakeFiles/test_memory.dir/src/test_writing.o: CMakeFiles/test_memory.dir/flags.make
CMakeFiles/test_memory.dir/src/test_writing.o: /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/src/test_writing.cpp
CMakeFiles/test_memory.dir/src/test_writing.o: CMakeFiles/test_memory.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_memory.dir/src/test_writing.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_memory.dir/src/test_writing.o -MF CMakeFiles/test_memory.dir/src/test_writing.o.d -o CMakeFiles/test_memory.dir/src/test_writing.o -c /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/src/test_writing.cpp

CMakeFiles/test_memory.dir/src/test_writing.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_memory.dir/src/test_writing.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/src/test_writing.cpp > CMakeFiles/test_memory.dir/src/test_writing.i

CMakeFiles/test_memory.dir/src/test_writing.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_memory.dir/src/test_writing.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/src/test_writing.cpp -o CMakeFiles/test_memory.dir/src/test_writing.s

CMakeFiles/test_memory.dir/src/MarkerDict.o: CMakeFiles/test_memory.dir/flags.make
CMakeFiles/test_memory.dir/src/MarkerDict.o: /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/src/MarkerDict.cpp
CMakeFiles/test_memory.dir/src/MarkerDict.o: CMakeFiles/test_memory.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/test_memory.dir/src/MarkerDict.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_memory.dir/src/MarkerDict.o -MF CMakeFiles/test_memory.dir/src/MarkerDict.o.d -o CMakeFiles/test_memory.dir/src/MarkerDict.o -c /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/src/MarkerDict.cpp

CMakeFiles/test_memory.dir/src/MarkerDict.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_memory.dir/src/MarkerDict.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/src/MarkerDict.cpp > CMakeFiles/test_memory.dir/src/MarkerDict.i

CMakeFiles/test_memory.dir/src/MarkerDict.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_memory.dir/src/MarkerDict.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/src/MarkerDict.cpp -o CMakeFiles/test_memory.dir/src/MarkerDict.s

# Object files for target test_memory
test_memory_OBJECTS = \
"CMakeFiles/test_memory.dir/src/test_writing.o" \
"CMakeFiles/test_memory.dir/src/MarkerDict.o"

# External object files for target test_memory
test_memory_EXTERNAL_OBJECTS =

/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: CMakeFiles/test_memory.dir/src/test_writing.o
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: CMakeFiles/test_memory.dir/src/MarkerDict.o
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: CMakeFiles/test_memory.dir/build.make
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_gapi.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_stitching.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_aruco.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_bgsegm.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_bioinspired.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_ccalib.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_dnn_objdetect.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_dnn_superres.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_dpm.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_face.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_freetype.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_fuzzy.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_hfs.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_img_hash.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_intensity_transform.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_line_descriptor.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_mcc.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_quality.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_rapid.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_reg.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_rgbd.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_saliency.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_signal.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_stereo.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_structured_light.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_superres.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_surface_matching.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_tracking.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_videostab.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_wechat_qrcode.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_xfeatures2d.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_xobjdetect.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_xphoto.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_shape.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_highgui.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_datasets.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_plot.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_text.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_ml.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_phase_unwrapping.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_optflow.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_ximgproc.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_video.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_videoio.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_imgcodecs.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_objdetect.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_calib3d.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_dnn.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_features2d.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_flann.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_photo.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_imgproc.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: /usr/local/lib/libopencv_core.so.4.10.0
/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory: CMakeFiles/test_memory.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_memory.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_memory.dir/build: /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/bin/test_memory
.PHONY : CMakeFiles/test_memory.dir/build

CMakeFiles/test_memory.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_memory.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_memory.dir/clean

CMakeFiles/test_memory.dir/depend:
	cd /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/build /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/build /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/build/CMakeFiles/test_memory.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_memory.dir/depend

