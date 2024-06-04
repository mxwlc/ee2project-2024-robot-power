# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = /opt/clion-2024.1.2/bin/cmake/linux/x64/bin/cmake

# The command to remove a file.
RM = /opt/clion-2024.1.2/bin/cmake/linux/x64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp

# Include any dependencies generated for this target.
include CMakeFiles/camera_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/camera_test.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/camera_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/camera_test.dir/flags.make

CMakeFiles/camera_test.dir/src/webcam_test.o: CMakeFiles/camera_test.dir/flags.make
CMakeFiles/camera_test.dir/src/webcam_test.o: src/webcam_test.cpp
CMakeFiles/camera_test.dir/src/webcam_test.o: CMakeFiles/camera_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/camera_test.dir/src/webcam_test.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/camera_test.dir/src/webcam_test.o -MF CMakeFiles/camera_test.dir/src/webcam_test.o.d -o CMakeFiles/camera_test.dir/src/webcam_test.o -c /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/src/webcam_test.cpp

CMakeFiles/camera_test.dir/src/webcam_test.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/camera_test.dir/src/webcam_test.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/src/webcam_test.cpp > CMakeFiles/camera_test.dir/src/webcam_test.i

CMakeFiles/camera_test.dir/src/webcam_test.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/camera_test.dir/src/webcam_test.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/src/webcam_test.cpp -o CMakeFiles/camera_test.dir/src/webcam_test.s

CMakeFiles/camera_test.dir/src/marker_dict.o: CMakeFiles/camera_test.dir/flags.make
CMakeFiles/camera_test.dir/src/marker_dict.o: src/marker_dict.cpp
CMakeFiles/camera_test.dir/src/marker_dict.o: CMakeFiles/camera_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/camera_test.dir/src/marker_dict.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/camera_test.dir/src/marker_dict.o -MF CMakeFiles/camera_test.dir/src/marker_dict.o.d -o CMakeFiles/camera_test.dir/src/marker_dict.o -c /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/src/marker_dict.cpp

CMakeFiles/camera_test.dir/src/marker_dict.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/camera_test.dir/src/marker_dict.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/src/marker_dict.cpp > CMakeFiles/camera_test.dir/src/marker_dict.i

CMakeFiles/camera_test.dir/src/marker_dict.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/camera_test.dir/src/marker_dict.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/src/marker_dict.cpp -o CMakeFiles/camera_test.dir/src/marker_dict.s

# Object files for target camera_test
camera_test_OBJECTS = \
"CMakeFiles/camera_test.dir/src/webcam_test.o" \
"CMakeFiles/camera_test.dir/src/marker_dict.o"

# External object files for target camera_test
camera_test_EXTERNAL_OBJECTS =

bin/camera_test: CMakeFiles/camera_test.dir/src/webcam_test.o
bin/camera_test: CMakeFiles/camera_test.dir/src/marker_dict.o
bin/camera_test: CMakeFiles/camera_test.dir/build.make
bin/camera_test: /usr/local/lib/libopencv_gapi.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_stitching.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_aruco.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_bgsegm.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_bioinspired.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_ccalib.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_dnn_objdetect.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_dnn_superres.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_dpm.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_face.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_freetype.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_fuzzy.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_hfs.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_img_hash.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_intensity_transform.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_line_descriptor.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_mcc.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_quality.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_rapid.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_reg.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_rgbd.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_saliency.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_signal.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_stereo.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_structured_light.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_superres.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_surface_matching.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_tracking.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_videostab.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_wechat_qrcode.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_xfeatures2d.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_xobjdetect.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_xphoto.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_shape.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_highgui.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_datasets.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_plot.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_text.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_ml.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_phase_unwrapping.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_optflow.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_ximgproc.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_video.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_videoio.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_imgcodecs.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_objdetect.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_calib3d.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_dnn.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_features2d.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_flann.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_photo.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_imgproc.so.4.10.0
bin/camera_test: /usr/local/lib/libopencv_core.so.4.10.0
bin/camera_test: CMakeFiles/camera_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable bin/camera_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/camera_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/camera_test.dir/build: bin/camera_test
.PHONY : CMakeFiles/camera_test.dir/build

CMakeFiles/camera_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/camera_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/camera_test.dir/clean

CMakeFiles/camera_test.dir/depend:
	cd /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp /home/maxwe/Documents/ee2project24/ee2project-2024-robot-power/computer_vision_cpp/CMakeFiles/camera_test.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/camera_test.dir/depend

