//
// Created by maxwe on 07/06/24.
//

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#include "../include/square_overlay.hpp"

namespace overlay
{
	square_overlay::square_overlay(std::vector<cv::Point2f>& corners_in)
	{
		cv::Point2f p1 = corners_in[0];
		cv::Point2f p2 = corners_in[1];

		side_length = (int)cv::norm(p1 - p2);
		corners = corners_in;
		window_height = WINDOW_HEIGHT;
		window_width = WINDOW_WIDTH;
	}

	square_overlay::square_overlay(int side_length_in)
	{
		if (DEBUG_FLAG) std::cout << "square_overlay(int side_length)\n";
		auto window_width_f = (float)window_width;
		auto window_height_f = (float)window_height;
		auto x_0 = (float)(window_width_f / 2);
		auto y_0 = (float)(window_height_f / 2);
		auto half_len = (float)((float)side_length_in / (float)2);
		cv::Point2f origin = cv::Point2f(x_0, y_0);

		std::vector<cv::Point2f> box_vertices = { cv::Point2f(half_len, half_len), cv::Point2f(half_len, -half_len),
			cv::Point2f(-half_len, -half_len), cv::Point2f(-half_len, half_len) };
		for (auto& vertex : box_vertices)
		{
			vertex += origin;
		}
		corners = box_vertices;
		side_length = side_length_in;
	}

	bool square_overlay::within_bounds(std::vector<cv::Point2f>& marker)
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

	bool square_overlay::point_in_bounds(cv::Point2f v)
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

	std::vector<cv::Point2f> square_overlay::getCorners() const
	{
		return corners;
	}

	void square_overlay::draw(cv::Mat& m)
	{
		// God knows why this is an issue :3

		if (DEBUG_FLAG) std::cout << "Draw Square Overlay\n";

		// Poly lines only accepts integer points
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
	std::string square_overlay::print() const
	{
		std::string output;
		output = std::string(quote(square_overlay)) + "\n";
		std::vector<cv::Point2f> vertices = getCorners();
		for (auto vertex : vertices)
		{
			output += ("(" + std::to_string(vertex.x) + ", " + std::to_string(vertex.y) + ")\n");
		}
		return output;
	}

	std::ostream& operator<<(std::ostream& os, const square_overlay& so)
	{
		std::string output = so.print();
		return os << output;
	}
}