//
// Created by maxwe on 08/06/24.
//
#ifndef EE2_COMPUTER_VISION_OVERLAY_HPP
#define EE2_COMPUTER_VISION_OVERLAY_HPP

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

#define quote(x) #x

#pragma once
namespace overlay {

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

		virtual bool point_in_bounds();

		virtual std::string print() const;

        virtual void draw();


    };

    std::ostream& operator<<(std::ostream& os, overlay const& o);
}
#endif //EE2_COMPUTER_VISION_OVERLAY_HPP
