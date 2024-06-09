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

class target
{
 public:
	std::vector<std::vector<cv::Point2f>> target_coords;
	std::vector<int> target_id;

	target()
	{
		cv::Point2f zero = cv::Point2f(0, 0);
		std::vector<cv::Point2f> marker = { zero };
		std::vector<std::vector<cv::Point2f>> mmarker = { marker };

		target_coords = mmarker;
		target_id = { 0 };
	}
	target(std::vector<std::vector<cv::Point2f>> target_coords_, std::vector<int> target_id_)
		: target_id(target_id_), target_coords(target_coords_)
	{
	}

	void set_target(std::vector<std::vector<cv::Point2f>> target_coords_, std::vector<int> target_id_)
	{
		target_coords = target_coords_;
		target_id = target_id_;

	}

	std::vector<std::vector<cv::Point2f>> get_target_coords()
	{
		return target_coords;
	}

	std::vector<int> get_target_id()
	{
		return target_id;
	}
};

std::vector<int> translate_found_markers(std::unique_ptr<dictionary::marker_dict>& md, std::vector<int>& input_ids)
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
	bool target_flag = false;
	bool ids_flag = false;
	bool location_flag = false;
	cv::Mat frame;

	overlay::square_overlay sq_o(200);
	overlay::column_overlay c_o(200);

	if (overlay::DEBUG_FLAG) std::cout << "Square Overlay Initialised\n";
	std::unique_ptr<dictionary::marker_dict> md(new dictionary::marker_dict("marker_dict"));

	int id;
	std::cout << "Target id = ";
	std::cin >> id;

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
	cv::aruco::DetectorParameters detector_params = cv::aruco::DetectorParameters();
	cv::aruco::Dictionary dict = cv::aruco::getPredefinedDictionary(ARUCO_DICTIONARY);
	cv::aruco::ArucoDetector detector(dict, detector_params);

	std::cout << "Press any Key to Start\n";

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

		std::vector<int> marker_ids;
		std::vector<std::vector<cv::Point2f>> marker_corners, rejected_candidates;

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
		int t_j;

		std::vector<std::vector<cv::Point2f>> t_coord;

		if (!marker_ids.empty())
		{
			for (int j = 0; j < marker_ids.size(); j++)
			{
				if (marker_ids[j] == id)
				{
					t_coord = { marker_corners[j] };
//						target_marker->set_target({ marker_corners[j] }, std::vector<int>(marker_ids[j]))
					std::cout << "Found Target " << id << "\n";
				}
			}
		}

		if (overlay::DEBUG_FLAG)
		{
			for (int i = 0; i < marker_ids.size(); i++)
			{
				std::cout << "\nid = " << marker_ids[i] << "\nSquare : " << sq_o.within_bounds(marker_corners[i])
						  << "\nColumn : " << c_o.within_bounds(marker_corners[i]) << "\nPos : "
						  << overlay::position_translation[c_o.marker_position(marker_corners[i])];
			}
		}

		if (location_flag)
		{
			if (target_flag)
			{
				marker_corners = t_coord;
				marker_ids = { id };

			}
			if (!marker_corners.empty())
			{
				for (int i = 0; i < marker_ids.size(); i++)
				{
					std::cout << i << ") ";
					std::cout << "id=" << marker_ids[i] << " pos : "
							  << overlay::position_translation[c_o.marker_position(marker_corners[i])] << "\n";
				}
			}
		}
		if (target_flag)
		{
			if (!marker_ids.empty())
			{
				std::vector<std::vector<cv::Point2f>> t_corner = t_coord;
				std::vector<int> t_id = { id };
				std::vector<int> t_state = translate_found_markers(md, t_id);
				if (!t_corner.empty() && !t_id.empty())
				{
					if (ids_flag) cv::aruco::drawDetectedMarkers(frame, t_corner, t_id);
					else cv::aruco::drawDetectedMarkers(frame, t_corner, t_state);
				}
			}
		}
		else
		{
			if (ids_flag) cv::aruco::drawDetectedMarkers(frame, marker_corners, marker_ids);
			else cv::aruco::drawDetectedMarkers(frame, marker_corners, marker_states);
		}
		cv::imshow("Live", frame);

		// --  close window when key pressed --
		if (key_press == (int)'i')
		{
			std::cout << "Toggle ID/METHOD\n";
			ids_flag = !ids_flag;
		}
		else if (key_press == (int)'l')
		{
			std::cout << "Toggle Locations (l/r/m)\n";
			location_flag = !location_flag;
		}
		else if (key_press == (int)'t')
		{
			std::cout << "Toggle Target\n";
			target_flag = !target_flag;
		}
		else if (key_press >= 0)
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
