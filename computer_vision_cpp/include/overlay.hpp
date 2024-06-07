//
// Created by maxwe on 08/06/24.
//

#pragma once

#ifndef EE2_COMPUTER_VISION_OVERLAY_HPP
#define EE2_COMPUTER_VISION_OVERLAY_HPP

#include <iostream>
#include <opencv2/core.hpp>


class overlay {
protected:
    int height;
    int length;
    std::vector<cv::Point2i> *corners;
public:
    overlay();

    virtual ~overlay();

    virtual bool within_bounds() = 0;
};


#endif //EE2_COMPUTER_VISION_OVERLAY_HPP
