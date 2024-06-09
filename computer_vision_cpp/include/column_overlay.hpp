//
// Created by maxwe on 08/06/24.
//

#ifndef EE2_COMPUTER_VISION_COLUMN_OVERLAY_HPP
#define EE2_COMPUTER_VISION_COLUMN_OVERLAY_HPP

#include "overlay.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

namespace overlay
{
	class column_overlay : public overlay
	{
	 private:
		int padding;

	 public:
		column_overlay();

		explicit column_overlay(int padding);

		[[nodiscard]] int GetPadding() const;

		uchar position(cv::Point2f& pt);

		uchar marker_position(std::vector<cv::Point2f>& m);

		std::string print() const override;

		bool within_bounds(std::vector<cv::Point2f>& marker);

		bool point_in_bounds(cv::Point2f point) const;

		void draw(cv::Mat& m);
	};

	std::ostream& operator<<(std::ostream& os, column_overlay const& o);
}
#endif //EE2_COMPUTER_VISION_COLUMN_OVERLAY_HPP
