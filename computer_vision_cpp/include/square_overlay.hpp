//
// Created by maxwe on 07/06/24.
//
#pragma once

#ifndef EE2_COMPUTER_VISION_SQUARE_OVERLAY_HPP
#define EE2_COMPUTER_VISION_SQUARE_OVERLAY_HPP

#include<opencv2/core.hpp>
#include<map>
#include<iostream>
#include "overlay.hpp"


class square_overlay : public overlay {
public:
    square_overlay() = default;

    square_overlay(std::vector<cv::Point2i> &);

    square_overlay(int side_length);

    bool within_bounds(std::vector<cv::Point2i> &marker);

    void draw(cv::Mat &m);

    std::vector<cv::Point2i> getCorners();

private:
    std::vector<cv::Point2i> corners;
};


#endif //EE2_COMPUTER_VISION_SQUARE_OVERLAY_HPP
