//
// Created by maxwe on 25/05/24.
//

#include "../include/column_overlay.hpp"
#include "../include/marker_dict.hpp"
#include "../include/square_overlay.hpp"
#include <ctime>
#include <iostream>
#include <map>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <sstream>

#define MARKER_EDGE_SIZE = 200
#define BORDER_SIZE = 1
#define ARUCO_DICTIONARY cv::aruco::DICT_5X5_1000

std::vector<int> translate_found_markers(std::shared_ptr<dictionary::marker_dict>& md, std::vector<int>& input_ids)
{
	std::vector<int> marker_states;
	for (int input_id : input_ids)
	{
		marker_states.push_back(int(md->marker_translate(input_id)));
	}
	return marker_states;
}

std::string gen_file_name_formatted(const std::string& key, const std::string& file_type)
{
	std::time_t t = std::time(nullptr);
	std::tm* now = std::localtime(&t);
	std::ostringstream datestream;
	datestream << std::put_time(now, "-%d%m%y-%H%M");
	return "output/" + key + datestream.str() + "." + file_type;
}

int main()
{
	overlay::DEBUG_FLAG = false;

	cv::Mat frame;

	overlay::square_overlay sq_o(200);
	overlay::column_overlay c_o(200);

	if (overlay::DEBUG_FLAG) std::cout << "Square Overlay Initialised\n";
	std::shared_ptr<dictionary::marker_dict> md(new dictionary::marker_dict("marker_dict"));


	// -- verify marker dict has been loaded properly --
	if (md->size_of_map() <= 1)
	{
		std::cerr << "Error : marker_dict no initialised properly\n";
		return 1;
	}

	// -- Video Capture Init --
	cv::VideoCapture cap;
	cap.open(-1);
	if (!cap.isOpened())
	{
		std::cerr << "Error : Unable to open Camera\n";
		return -1;
	}

	// -- Main Loop --
	std::cout << "Press any key to terminate\n";
	for (;;)
	{
		cap.read(frame);
		int key_press = cv::waitKey(5);
		// -- validate frame --
		if (frame.empty())
		{
			std::cerr << "Error : Blank Frame Grabbed\n";
			break;
		}

		if(overlay::DEBUG_FLAG){
			std::cout << frame.type() << std::endl;
		}


		std::vector<int> marker_ids;
		std::vector<std::vector<cv::Point2f>> marker_corners, rejected_candidates;
		cv::aruco::DetectorParameters detector_params = cv::aruco::DetectorParameters();
		cv::aruco::Dictionary dict = cv::aruco::getPredefinedDictionary(ARUCO_DICTIONARY);
		cv::aruco::ArucoDetector detector(dict, detector_params);
		detector.detectMarkers(frame, marker_corners, marker_ids, rejected_candidates);
		if (!marker_ids.empty() && overlay::DEBUG_FLAG)
		{
			for (int i = 0; i < marker_ids.size(); ++i)
			{
				std::cout << marker_ids[i] << '\n';
				int k = 0;
				for (auto corner : marker_corners[i])
				{
					std::cout << "Point " << k << " : " << "( " << corner.x << ", " << corner.y << ")" << "\n";
					k++;
				}
			}
		}

		std::vector<int> marker_states = translate_found_markers(md, marker_ids);

		// Draw Polygon Boundaries
		sq_o.draw(frame);
		c_o.draw(frame);

		if (overlay::DEBUG_FLAG)
		{
			for (int i = 0; i < marker_ids.size(); i++)
			{
				std::cout << "\nid = " << marker_ids[i] << "\nSquare : " << sq_o.within_bounds(marker_corners[i])
						  << "\nColumn : " << c_o.within_bounds(marker_corners[i]);
			}
		}
		for (int i = 0; i < marker_ids.size(); i++)
		{
			std::cout << "\nid = " << marker_ids[i] << "\nSquare : " << sq_o.within_bounds(marker_corners[i]);
		}
		cv::aruco::drawDetectedMarkers(frame, marker_corners, marker_ids);
//		cv::aruco::drawDetectedMarkers(frame, marker_corners, marker_states);

		cv::imshow("Live", frame);

		// --  close window when key pressed --
		if (key_press >= 0)
		{
			if (key_press == int('s'))
			{
				std::string format_str = gen_file_name_formatted("webcam", "bmp");
				std::cout << "Saving to " << format_str << "\n";
				cv::imwrite(format_str, frame);
			}
			break;
		}
	}

	return 0;
}
