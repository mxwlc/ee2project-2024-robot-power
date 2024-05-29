//
// Created by maxwe on 25/05/24.
//
#include <cmath>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
// #include <opencv2/objdetect/aruco_board.hpp>
#include <map>

#define MARKER_EDGE_SIZE = 200
#define BORDER_SIZE = 1
#define ARUCO_DICTIONARY cv::aruco::DICT_5X5_1000

int main()
{
    cv::Mat frame;

    // -- Video Capture Init --
    cv::VideoCapture cap;
    cap.open(0);
    if (!cap.isOpened())
    {
        std::cerr << "Error : Unable to open Camera\n";
        return -1;
    }

    // -- Main Loop --
    std::cout << "Press any key to terminate\n";
    for (;;)
    {
        cap.read(frame);

        // -- validate frame --
        if (frame.empty())
        {
            std::cerr << "Error : Blank Frame Grabbed\n";
            break;
        }

        if (cv::waitKey(5) >= 0)
        {
            break;
        }

        std::vector<int> marker_ids;
        std::vector<std::vector<cv::Point2f>> marker_corners, rejected_candidates;
        cv::aruco::DetectorParameters detector_params = cv::aruco::DetectorParameters();
        cv::aruco::Dictionary dict = cv::aruco::getPredefinedDictionary(ARUCO_DICTIONARY);
        cv::aruco::ArucoDetector detector(dict, detector_params);
        detector.detectMarkers(frame, marker_corners, marker_ids, rejected_candidates);
        cv::aruco::drawDetectedMarkers(frame, marker_corners, marker_ids);

        cv::imshow("Live", frame);
    }

    return 0;
}
