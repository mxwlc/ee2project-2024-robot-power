//
// Created by maxwe on 08/06/24.
//

#ifndef EE2_COMPUTER_VISION_COLUMN_OVERLAY_HPP
#define EE2_COMPUTER_VISION_COLUMN_OVERLAY_HPP

#include "overlay.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

namespace overlay {
    class column_overlay : public overlay {
    private:
        int left_pad;

        int right_pad;

    public:
		column_overlay();

        column_overlay(int left_pad, int right_pad);

		int GetLeftPad() const;

		int GetRightPad() const;

		std::string print() const;

		bool within_bounds(std::vector<cv::Point2f>& marker);

        void draw(cv::Mat& m);
    };

	std::ostream& operator<<(std::ostream& os, column_overlay const& o);
}
#endif //EE2_COMPUTER_VISION_COLUMN_OVERLAY_HPP
