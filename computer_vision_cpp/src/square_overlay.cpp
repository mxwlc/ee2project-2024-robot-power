//
// Created by maxwe on 07/06/24.
//

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#include "../include/square_overlay.hpp"

// TODO : Simple position location

namespace overlay
{
	square_overlay::square_overlay(std::vector<cv::Point2f>& corners_in)
	{

		corners = corners_in;
		window_height = WINDOW_HEIGHT;
		window_width = WINDOW_WIDTH;
	}

	square_overlay::square_overlay(int side_length)
	{
		if (DEBUG_FLAG) std::cout << "square_overlay(int side_length)\n";
		auto window_width_f = (float)window_width;
		auto window_height_f = (float)window_height;
		auto x_0 = (float)(window_width_f / 2);
		auto y_0 = (float)(window_height_f / 2);
		auto half_len = (float)(side_length / 2);
		cv::Point2f origin = cv::Point2f(x_0, y_0);

		std::vector<cv::Point2f> box_vertices = { cv::Point2f(half_len, half_len), cv::Point2f(half_len, -half_len),
			cv::Point2f(-half_len, -half_len), cv::Point2f(-half_len, half_len) };
		for (auto& vertex : box_vertices)
		{
			vertex += origin;
		}
		corners = box_vertices;
	}

	bool square_overlay::within_bounds(std::vector<cv::Point2f>& marker)
	{
		for (cv::Point2f vertex : marker)
		{
			bool inside = false;
			for (int i = 0; i < corners.size(); i++)
			{
				/*  *---------*
				 *  |         |
				 *  |    x----|
				 *  |    |    |
				 *  |    |    |
				 *  *---------*
				 */
				cv::Point2f p1 = corners[i];
				cv::Point2f p2 = corners[i + 1];
				if (i == corners.size() - 1)
				{
					p2 = corners[0];
				}
				if (vertex.y > std::min(p1.y, p2.y))
				{
					if (vertex.y <= std::max(p1.y, p2.y))
					{
						if (vertex.x <= std::max(p1.x, p2.x))
						{
							if (p1.y != p2.y)
							{
								float x_intercept = ((vertex.y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y)) + p1.x;
								if (p1.x == p2.x || vertex.x <= x_intercept)
								{
									inside = !inside;
								}
							}
						}
					}
				}
			}
		}
		return false;
	}

	std::vector<cv::Point2f> square_overlay::getCorners()
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

}