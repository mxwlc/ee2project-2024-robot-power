//
// Created by maxwe on 08/06/24.
//

#include "column_overlay.hpp"
// TODO : Simple position location
namespace overlay
{
	column_overlay::column_overlay()
	{
		padding = 0;
	}

	column_overlay::column_overlay(int padding_) : padding(padding_)
	{
	}

	bool column_overlay::within_bounds(std::vector<cv::Point2f>& marker)
	{
		bool inside = false;
		for (cv::Point2f vertex : marker)
		{
			inside = point_in_bounds(vertex);
			if (!inside) return false;
		}
		return inside;
	}

	void column_overlay::draw(cv::Mat& m)
	{
		auto padding_f = (float)padding;
		auto window_width = (float)WINDOW_WIDTH;
		auto window_height = (float)WINDOW_HEIGHT;
		if (DEBUG_FLAG) std::cout << "Draw Column Overlay\n";
		cv::line(m, cv::Point2f(padding_f, 0.0), cv::Point2f(padding_f, window_height), cv::Scalar(0, 255, 0), 2);
		cv::line(m, cv::Point2f((window_width - padding_f), 0.0), cv::Point2f(
			window_width - padding_f, window_height), cv::Scalar(0, 255, 0), 2);
	}
	int column_overlay::GetPadding() const
	{
		return padding;
	}
	std::string column_overlay::print() const
	{
		std::string output;
		output = std::string("--Column Overlay--\n") + std::string("Padding : ") + std::to_string(GetPadding()) + "\n";
		return output;
	}
	bool column_overlay::point_in_bounds(cv::Point2f point) const
	{
		if ((float)padding <= point.x && point.x <= (float)(window_width - padding))
		{
			return true;
		}
		return false;
	}

	std::ostream& operator<<(std::ostream& os, const column_overlay& o)
	{
		std::string output;
		output = o.print();
		return os << output;
	}
}