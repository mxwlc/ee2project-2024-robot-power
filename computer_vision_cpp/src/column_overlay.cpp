//
// Created by maxwe on 08/06/24.
//

#include "column_overlay.hpp"
column_overlay::column_overlay() {
    left_pad=0;
    right_pad=0;
}

column_overlay::column_overlay(int left_pad_, int right_pad_) : left_pad(left_pad_), right_pad(right_pad_){}

bool column_overlay::within_bounds(std::vector<cv::Point2i> &marker) {
    bool inside = false;
    for (cv::Point2i vertex: marker) {
        inside = (vertex.x >= left_pad && vertex.x <= right_pad);
    }
    return inside;
}


void column_overlay::draw(cv::Mat &m) {
    if(DEBUG_FLAG) std::cout << "Draw Column Overlay\n";
    cv::line(m, {left_pad, 0}, {left_pad, WINDOW_HEIGHT}, cv::Scalar(0, 255, 0), 2);
    cv::line(m, {WINDOW_WIDTH - right_pad, 0}, {WINDOW_WIDTH - right_pad, WINDOW_HEIGHT}, cv::Scalar(0, 255, 0), 2);

}



