#!/bin/bash
# Install minimal prerequisites (Ubuntu 18.04 as reference)
sudo apt update && sudo apt install -y cmake g++ wget unzip
 
# Download and unpack sources
wget -O opencv.zip https://github.com/opencv/opencv/archive/4.x.zip
wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/4.x.zip
unzip opencv.zip
unzip opencv_contrib.zip
 
# Create build directory and switch into it
mkdir -p build && cd build
 
# Configure
cmake -D OPENCV_EXTRA_MODULES_PATH=../opencv_contrib-4.x/modules/aruco \
      -D CMAKE_BUILD_TYPE=RELEASE \
      -D BUILD_TESTS=OFF \
      -D INSTALL_PYTHON_EXAMPLES=OFF \
      -D BUILD_EXAMPLES=OFF\
	  ../opencv-4.x
 
# Build
make -j4

# run sudo make install
echo "dont leave directory and input sudo make install"
