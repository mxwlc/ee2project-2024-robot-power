//
// Created by maxwe on 07/06/24.
//

#ifndef EE2_COMPUTER_VISION_OVERLAY_HPP
#define EE2_COMPUTER_VISION_OVERLAY_HPP

#include<opencv2/core.hpp>
#include<map>
#include<iostream>


class overlay {
protected:
    std::vector<cv::Point2i> corners;
public:
    virtual bool within_bounds(){}
};

class square_overlay : public overlay {
public:
    square_overlay() = default;

    square_overlay(std::vector<cv::Point2i>) {}

    ~square_overlay() = default;
};


#endif //EE2_COMPUTER_VISION_OVERLAY_HPP
