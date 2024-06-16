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

namespace overlay
{
	class SquareOverlay : public Overlay
	{
	 public:
		SquareOverlay() = default;

		explicit SquareOverlay(std::vector<cv::Point2f>&);

		explicit SquareOverlay(int side_length_);

		bool within_bounds(std::vector<cv::Point2f>& marker);

		bool point_in_bounds(cv::Point2f v);

		void draw(cv::Mat& m);

		std::vector<cv::Point2f> getCorners() const;

		std::string print() const;
	 private:
		std::vector<cv::Point2f> corners;

		int side_length;
	};
	std::ostream& operator<<(std::ostream& os, SquareOverlay const& so);
}
#endif //EE2_COMPUTER_VISION_SQUARE_OVERLAY_HPP
