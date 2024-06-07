//
// Created by maxwe on 25/05/24.
//

#include "../include/marker_dict.hpp"
#include <ctime>
#include <iostream>
#include <map>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <sstream>

#define MARKER_EDGE_SIZE = 200
#define BORDER_SIZE = 1
#define ARUCO_DICTIONARY cv::aruco::DICT_5X5_1000


std::vector<int> translate_found_markers(std::shared_ptr<marker_dict> &md, std::vector<int> &input_ids) {
    std::vector<int> marker_states;
    for (int i = 0; i < input_ids.size(); i++) {
        int temp_id = input_ids[i];
        marker_states.push_back(int(md->marker_translate(input_ids[i])));
    }
    return marker_states;
}

void draw_center(cv::Mat &m, int len) {
    int half_len = len / 2;
    cv::Size m_size = m.size();
    int y_0 = m_size.height / 2;
    int x_0 = m_size.width / 2;
    cv::Point2i center(x_0, y_0);
    std::vector<cv::Point2i> box_add = {{half_len,  half_len},
                                        {half_len,  -half_len},
                                        {-half_len, -half_len},
                                        {-half_len, half_len}};

    cv::line(m, (center + box_add[0]), (center + box_add[1]), {255, 255, 255}, 2);
    cv::line(m, (center + box_add[1]), (center + box_add[2]), {255, 255, 255}, 2);
    cv::line(m, (center + box_add[2]), (center + box_add[3]), {255, 255, 255}, 2);
    cv::line(m, (center + box_add[3]), (center + box_add[0]), {255, 255, 255}, 2);
}

std::string gen_file_name_formatted(std::string key, std::string file_type) {
    std::time_t t = std::time(nullptr);
    std::tm *now = std::localtime(&t);
    std::ostringstream datestream;
    datestream << std::put_time(now, "-%d%m%y-%H%M");
    return "output/" + key + datestream.str() + "." + file_type;
}


int main() {
    cv::Mat frame;
    std::shared_ptr<marker_dict> md(new marker_dict("marker_dict"));

    // -- verify marker dict has been loaded properly --
    if (md->size_of_map() <= 1) {
        std::cerr << "Error : marker_dict no initialised properly\n";
        return 1;
    }

    // -- Video Capture Init --
    cv::VideoCapture cap;
    cap.open(0);
    if (!cap.isOpened()) {
        std::cerr << "Error : Unable to open Camera\n";
        return -1;
    }

    // -- Main Loop --
    std::cout << "Press any key to terminate\n";
    for (;;) {
        cap.read(frame);
        int key_press = cv::waitKey(5);
        // -- validate frame --
        if (frame.empty()) {
            std::cerr << "Error : Blank Frame Grabbed\n";
            break;
        }


        std::vector<int> marker_ids;
        std::vector<std::vector<cv::Point2f>> marker_corners, rejected_candidates;
        cv::aruco::DetectorParameters detector_params = cv::aruco::DetectorParameters();
        cv::aruco::Dictionary dict = cv::aruco::getPredefinedDictionary(ARUCO_DICTIONARY);
        cv::aruco::ArucoDetector detector(dict, detector_params);
        detector.detectMarkers(frame, marker_corners, marker_ids, rejected_candidates);
        if (!marker_ids.empty()) {
            for (int i = 0; i < marker_ids.size(); ++i) {
                std::cout << marker_ids[i] << '\n';
                int k = 0;
                for (auto corner: marker_corners[i]) {
                    std::cout << "Point " << k << " : " << "( " << corner.x << ", " << corner.y << ")" << "\n";
                    k++;
                }
            }
        }

        std::vector<int> marker_states = translate_found_markers(md, marker_ids);


        draw_center(frame, 100);
        cv::aruco::drawDetectedMarkers(frame, marker_corners, marker_states);

        cv::imshow("Live", frame);

        // --  close window when key pressed --
        if (key_press >= 0) {
            if (key_press == int('s')) {
                std::string format_str = gen_file_name_formatted("webcam", "bmp");
                std::cout << "Saving to " << format_str << "\n";
                cv::imwrite(format_str, frame);
            }
            break;
        }
    }

    return 0;
}
