//
// Created by maxwe on 07/06/24.
//

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#include "../include/square_overlay.hpp"

square_overlay::square_overlay(std::vector<cv::Point2i> &corners_in) {

    corners = corners_in;
    window_height = WINDOW_HEIGHT;
    window_width = WINDOW_WIDTH;
}

square_overlay::square_overlay(int side_length) {
    if(DEBUG_FLAG) std::cout << "square_overlay(int side_length)\n";

    cv::Point2i origin = cv::Point2i((window_width / 2), (window_height / 2));
    int half_len = side_length / 2;
    std::vector<cv::Point2i> box_add = {{half_len,  half_len},
                                        {half_len,  -half_len},
                                        {-half_len, -half_len},
                                        {-half_len, half_len}};
    for (auto &vertex: box_add) {
        vertex += origin;
    }
    corners = box_add;
}

bool square_overlay::within_bounds(std::vector<cv::Point2i> &marker) {
    for (cv::Point2i vertex: marker) {
        bool inside = false;
        for (int i = 0; i < corners.size(); i++) {
            /*  *---------*
             *  |         |
             *  |    x----|
             *  |    |    |
             *  |    |    |
             *  *---------*
             */
            cv::Point2i p1 = corners[i];
            cv::Point2i p2 = corners[i + 1];
            if (i == corners.size() - 1) {
                p2 = corners[0];
            }
            if (vertex.y > std::min(p1.y, p2.y)) {
                if (vertex.y <= std::max(p1.y, p2.y)) {
                    if (vertex.x <= std::max(p1.x, p2.x)) {
                        if (p1.y != p2.y) {
                            int x_intercept = ((vertex.y - p1.y) * (p2.x - p1.x) /
                                               (p2.y - p1.y)) + p1.x;
                            if (p1.x == p2.x || vertex.x <= x_intercept) {
                                inside = !inside;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

std::vector<cv::Point2i> square_overlay::getCorners() {
    return corners;
}

void square_overlay::draw(cv::Mat &m) {
    if (DEBUG_FLAG) std::cout << "Draw Square Overlay\n";
    if (!corners.empty()) {
        cv::polylines(m, corners, true, cv::Scalar(0, 255, 0), 2);
    }
}

