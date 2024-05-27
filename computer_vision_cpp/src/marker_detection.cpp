//
// Created by maxwe on 27/05/24.
//
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <opencv2/objdetect/aruco_board.hpp>
#include <opencv2/objdetect/aruco_detector.hpp>
#include <opencv2/opencv.hpp>

// TODO : Save map to local file for persistance

#define MARKER_EDGE_SIZE = 200
#define BORDER_SIZE = 1
#define ARUCO_DICTIONARY cv::aruco::DICT_5X5_1000


typedef enum
{
    STOP = 0,
    FORWARD = 1,
    BACKWARD = 2,
    TURN_L = 3,
    TURN_R = 4
} states;

int main()
{
    cv::Mat input_image;
    cv::Mat raw_image;
    int down_height = 900;
    int down_width = 1200;
    raw_image = cv::imread("arucofieldtest.jpg");
    cv::resize(raw_image, input_image, cv::Size(down_width,down_height), cv::INTER_LINEAR);
    cv::imshow("Test", input_image);
    std::cout << input_image.size() << std::endl;
    cv::waitKey(0);
    if (!input_image.data)
    {
        exit(1);
    }

    std::vector<int> marker_ids;
    std::cout << "1" << std::endl;
    std::vector<std::vector<cv::Point2f>> marker_corners, rejected_candidates;
    std::cout << "2" << std::endl;
    cv::aruco::DetectorParameters detector_params = cv::aruco::DetectorParameters();
    std::cout << "3" << std::endl;
    cv::aruco::Dictionary dict = cv::aruco::getPredefinedDictionary(ARUCO_DICTIONARY);
    std::cout << "4" << std::endl;
    cv::aruco::ArucoDetector detector(dict, detector_params);
    std::cout << "5" << std::endl;
    detector.detectMarkers(input_image, marker_corners, marker_ids, rejected_candidates);
    std::cout << "6" << std::endl;

    cv::Mat output_image;
    output_image = input_image.clone();
    cv::aruco::drawDetectedMarkers(output_image, marker_corners, marker_ids);
    cv::imshow("Detected", output_image);
    cv::waitKey(0);
    cv::imwrite("output/detected_image.bmp", output_image);
}
