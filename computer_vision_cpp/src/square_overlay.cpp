//
// Created by maxwe on 07/06/24.
//

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#include "../include/square_overlay.hpp"

namespace overlay
{
	SquareOverlay::SquareOverlay(std::vector<cv::Point2f>& corners_)
	{
		cv::Point2f p1 = corners_[0];
		cv::Point2f p2 = corners_[1];

		side_length = (int)cv::norm(p1 - p2);
		corners = corners_;
		window_height = WINDOW_HEIGHT;
		window_width = WINDOW_WIDTH;
	}

	SquareOverlay::SquareOverlay(int side_length_)
	{
		if (DEBUG_FLAG) std::cout << "square_overlay(int side_length)\n";
		side_length = side_length_;
		auto window_width_f = (float)window_width;
		auto window_height_f = (float)window_height;
		auto x_0 = (float)(window_width_f / 2);
		auto y_0 = (float)(window_height_f / 2);
		auto half_len = (float)((float)side_length_ / (float)2);
		cv::Point2f origin = cv::Point2f(x_0, y_0);

		std::vector<cv::Point2f> box_vertices = { cv::Point2f(half_len, half_len), cv::Point2f(half_len, -half_len),
			cv::Point2f(-half_len, -half_len), cv::Point2f(-half_len, half_len) };
		for (auto& vertex : box_vertices)
		{
			vertex += origin;
		}
		corners = box_vertices;
		side_length = side_length_;
	}

	bool SquareOverlay::within_bounds(std::vector<cv::Point2f>& marker)
	{
		bool inside = true;
		for (auto corner : marker)
		{
			inside = point_in_bounds(corner);
			if (!inside)
			{
				return inside;
			}
		}
		return inside;
	}

	bool SquareOverlay::point_in_bounds(cv::Point2f v)
	{
		cv::Point2f p1 = corners[1];
		cv::Point2f p2 = corners[3];
		if (DEBUG_FLAG)
		{
			std::cout << "\np1 : " << p1 << "\n";

			std::cout << "p2 : " << p2 << "\n";
			std::cout << v.x << "<" << p1.x << "\n" << v.x << ">" << p2.x << "\n" << v.y << "<" << p1.y << "\n" << v.y
					  << ">" << p2.y << "\n";
		}
		if (v.x < p1.x and v.x > p2.x and v.y > p1.y and v.y < p2.y)
		{
			return true;
		}
		return false;
	}

	std::vector<cv::Point2f> SquareOverlay::getCorners() const
	{
		return corners;
	}

	void SquareOverlay::draw(cv::Mat& m)
	{
		// God knows why this is an issue :3
		std::vector<cv::Point2i> IntCornersVec;
		cv::Mat FloatCorners(corners);
		cv::Mat IntCorners;
		FloatCorners.convertTo(IntCorners, CV_32S);
		IntCorners.copyTo(IntCornersVec);
		if (corners.size() > 1)
		{
			if (!IntCorners.empty())
			{
				cv::polylines(m, IntCorners, true, cv::Scalar(0, 255, 0), 2);
			}
		}
	}
	std::string SquareOverlay::print() const
	{
		std::string output;
		output = std::string(quote(SquareOverlay)) + "\n";
		std::vector<cv::Point2f> vertices = getCorners();
		for (auto vertex : vertices)
		{
			output += ("(" + std::to_string(vertex.x) + ", " + std::to_string(vertex.y) + ")\n");
		}
		return output;
	}

	std::ostream& operator<<(std::ostream& os, const SquareOverlay& so)
	{
		std::string output = so.print();
		return os << output;
	}
}