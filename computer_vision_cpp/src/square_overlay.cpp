//
// Created by maxwe on 07/06/24.
//

#include "../include/square_overlay.hpp"
// TODO : IMPLEMENT SQUARE OVERLAY FUNCTIONS

square_overlay::square_overlay(const std::vector<cv::Point2i>& corners_in, cv::Mat &m){
    *corners = corners_in;
    cv::Size t_size = m.size();
    height = t_size.height;
    length = t_size.width;
}

bool square_overlay::within_bounds(std::vector<cv::Point2i> &marker) {
    // PLACEHOLDER
    for(cv::Point2i point : marker){
        std::cout << "(" << point.x << ", " << point.y << ")\n";
    }
    return false;
}

