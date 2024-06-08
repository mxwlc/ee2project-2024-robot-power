//
// Created by maxwe on 08/06/24.
//

#include "column_overlay.hpp"
// TODO : Simple position location
namespace overlay
{
	column_overlay::column_overlay()
	{
		left_pad = 0;
		right_pad = 0;
	}

	column_overlay::column_overlay(int left_pad_, int right_pad_) : left_pad(left_pad_), right_pad(right_pad_)
	{
	}

	bool column_overlay::within_bounds(std::vector<cv::Point2f>& marker)
	{
		bool inside = false;
		for (cv::Point2f vertex : marker)
		{
			inside = (vertex.x >= (float)left_pad && vertex.x <= (float)right_pad);
		}
		return inside;
	}

	void column_overlay::draw(cv::Mat& m)
	{
		auto left_pad_f = (float)left_pad;
		auto right_pad_f = (float)right_pad;
		auto window_width = (float)WINDOW_WIDTH;
		auto window_height = (float)WINDOW_HEIGHT;
		if (DEBUG_FLAG) std::cout << "Draw Column Overlay\n";
		cv::line(m, cv::Point2f(left_pad_f, 0.0), cv::Point2f(left_pad_f, window_height), cv::Scalar(0, 255, 0), 2);
		cv::line(m, cv::Point2f((window_width - right_pad_f), 0.0), cv::Point2f(
			window_width - right_pad_f, window_height), cv::Scalar(0, 255, 0), 2);
	}
	int column_overlay::GetLeftPad() const
	{
		return left_pad;
	}
	int column_overlay::GetRightPad() const
	{
		return right_pad;
	}
	std::string column_overlay::print() const
	{
		std::string output;
		output = std::string("--Column Overlay--\n") + std::string("Left Padding : ") + std::to_string(GetLeftPad())
				 + std::string("\n") + std::string("Right Padding : ") + std::to_string(GetRightPad())
				 + std::string("\n");
		return output;
	}

	std::ostream& operator<<(std::ostream& os, const column_overlay& o)
	{
		std::string output;
		output = o.print();
		return os << output;
	}
}