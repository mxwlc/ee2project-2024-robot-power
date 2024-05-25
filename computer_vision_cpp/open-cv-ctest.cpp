//
// Created by maxwe on 25/05/24.
//
//#include <opencv2/core.hpp>
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/opencv.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::Mat image = cv::imread("/home/maxwe/Documents/ee2project/ee2project-2024-robot-power/computer_vision_cpp/cleo0304.jpg" ,cv::IMREAD_COLOR);
    if(image.empty()) {
        std::cout << "Could not open or find the image" << std::endl;
        return -1;
    }
    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display Image", image);
    cv::waitKey(0);
    return 0;
}