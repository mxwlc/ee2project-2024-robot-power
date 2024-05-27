//
// Created by maxwe on 25/05/24.
//
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <opencv2/objdetect/aruco_board.hpp>
#include <map>



typedef enum {
    STOP,
    FORWARD,
    BACKWARD,
    TURN_L,
    TURN_R
} states ;

std::map<int, states> marker_map;


double length_between(std::shared_ptr<cv::Point2d> p1, std::shared_ptr<cv::Point2d> p2) {
    double x1(p1->x), x2(p2->x);
    double y1(p1->x), y2(p2->y);
    return std::sqrt(std::pow((x2 - x1), 2) + std::pow((y2 - y1), 2));
}





int main() {

    /*
     * Basic Assignments
     */
//    cv::Mat A, C;
//    cv::Mat B(A);
//    C = A;
//    cv::Mat D (A, cv::Rect(10,10,100,100));
//    cv::Mat E = A(cv::Range::all(), cv::Range(1,3));
//    cv::Mat F = A.clone();
//    cv::Mat G;
//    A.copyTo(G);
    uchar marker_id[] = {10, 20, 30, 40, 50};
    cv::Mat marker;
    cv::aruco::Dictionary dict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_5X5_250);
    std::cout << marker_id << " is of size " << sizeof(marker_id) << std::endl;
    for (int i = 0; i < sizeof(marker_id); ++i) {
        std::cout << "Marker ID = " << i << std::endl;
        cv::aruco::generateImageMarker(dict, marker_id[i], 200, marker, 1);
        std::string marker_filename =
                "/home/maxwe/Documents/ee2project/ee2project-2024-robot-power/computer_vision_cpp/markers/marker_id" +
                std::to_string(marker_id[i]) + ".bmp";
        cv::imwrite(marker_filename, marker);
    }


//    cv::aruco::generateImageMarker(dict, 10,200,image_output, 1);
//    std::string window_name;
//    window_name = "Aruco" + std::to_string(id);
//    cv::imshow(window_name, image_output);
//    cv::waitKey(0);
//    cv::imwrite("computer_vision_cpp/aruco_test.png",image_output);
//
//
//    cv::Mat M(2,2,CV_8UC3, cv::Scalar(0,0,255));
//    std::cout << "M = " << std::endl << M << std::endl;
//
//    cv::Mat zeroes = cv::Mat::zeros(2,2,CV_8U);
//    std::cout << "Z = " << std::endl << cv::format(zeroes, cv::Formatter::FMT_NUMPY) << std::endl;
//
//    std::shared_ptr<cv::Point2d> p1(new cv::Point2d (0.2f,0.2f));
//    std::shared_ptr<cv::Point2d> p2(new cv::Point2d(4.3f,2.1f));
//    std::cout << "D = " << length_between(p1,p2) << std::endl;

    return 0;
}
