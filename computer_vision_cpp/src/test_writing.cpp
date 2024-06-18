//
// Created by maxwe on 06/06/24.
//
#include <iostream>
#include <opencv2/core.hpp>
#include "../include/square_overlay.hpp"
#include "../include/overlay.hpp"
#include "../include/column_overlay.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

int main()
{
	overlay::DEBUG_FLAG = true;

	cv::Mat m = cv::Mat::zeros(overlay::WINDOW_HEIGHT, overlay::WINDOW_WIDTH, CV_8UC3);
	overlay::SquareOverlay sq_o(200);
	overlay::ColumnOverlay c_o(250);
	std::vector<cv::Point2f> v({cv::Point2f(300, 300), cv::Point2f(310, 300), cv::Point2f(300, 310), cv::Point2f(310, 310)});
	sq_o.draw(m);
	c_o.draw(m);
	std::cout << "------------------------\n";
	std::cout << c_o << std::endl;
	std::cout << "------------------------\n";
	std::cout << sq_o << std::endl;
	std::cout << "------------------------\n";
	std::cout << "------------------------\n" << "DEBUG SQUARE\n";
	for (auto corner : v)
	{
		std::cout << corner << "\n";
		cv::circle(m, corner, 10, cv::Scalar(0, 255, 0), 3);
	}
	std::cout << "DEBUG SQUARE - Done\n";

	std::cout << "------------------------\n" << "BOUNDARY SQUARE\n";
	auto corners = sq_o.getCorners();
	for (auto corner : corners)
	{
		std::cout << corner << "\n";
		cv::circle(m, corner, 10, cv::Scalar(0, 255, 0), 3);
	}
	std::cout << "BOUNDARY SQUARE - Done\n";

	cv::imshow("Test", m);


	std::cout << "------------------------\n";
	overlay::Overlay o = overlay::Overlay();
	std::cout << "\nOverlay : " << o << "\n";

	std::cout << std::boolalpha;
	std::cout << "Points ";
	for(auto vertex : v)
	{
		std::cout << " " << vertex;
	}

;


	std::cout << "\nAre within\n";
//	std::cout << "Point in Square : " << (bool)sq_o.point_in_bounds(p1) << "\n";
	std::cout << "Square Overlay : " << (bool)sq_o.within_bounds(v) << "\n";
	std::cout << "Column Overlay : " << (bool)c_o.within_bounds(v) << "\n";
	std::cout << std::noboolalpha;
	cv::waitKey(0);
}