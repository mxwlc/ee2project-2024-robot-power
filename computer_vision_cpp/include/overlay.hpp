//
// Created by maxwe on 08/06/24.
//

#pragma once

#ifndef EE2_COMPUTER_VISION_OVERLAY_HPP
#define EE2_COMPUTER_VISION_OVERLAY_HPP

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>



static int WINDOW_HEIGHT = 480;
static int WINDOW_WIDTH = 640;
static bool DEBUG_FLAG;

class overlay {
protected:
    int window_height;
    int window_width;
public:
    overlay();

    virtual ~overlay();

    virtual bool within_bounds();

    virtual void draw();


};


#endif //EE2_COMPUTER_VISION_OVERLAY_HPP
