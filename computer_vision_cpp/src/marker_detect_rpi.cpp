//
// Created by maxwe on 14/06/24.
//
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
#include <unistd.h>

#define MARKER_EDGE_SIZE = 200
#define BORDER_SIZE = 1
#define ARUCO_DICTIONARY cv::aruco::DICT_5X5_1000

std::map<uchar, std::string> direction_map = { std::pair<uchar, std::string>(0b0000, "Stop"),
	std::pair<uchar, std::string>(0b0001, "T_Right"), std::pair<uchar, std::string>(0b0010, "MV_Forward"),
	std::pair<uchar, std::string>(0b0100, "T_Left"), std::pair<uchar, std::string>(0b1111, "Invalid"),

};

uchar steering(std::vector<cv::Point2f>& target_marker, std::vector<int> id, overlay::ColumnOverlay& col_overlay)
{
	// 0b0001 :  TURN RIGHT
	// 0b0010 :  MIDDLE
	// 0b0100 : TURN LEFT
	// All other pos :1 STOP
	uchar pos = col_overlay.marker_position(target_marker);
	if (pos == 0b0010)
	{
		if (overlay::DEBUG_FLAG) std::cout << "Move Forward\n";
		return 0b0010;
	}
	if (pos == 0b0001 || pos == 0b0011)
	{
		if (overlay::DEBUG_FLAG) std::cout << "Turn Left\n";
		return 0b0100;
	}
	if (pos == 0b0100 | pos == 0b0110)
	{
		if (overlay::DEBUG_FLAG) std::cout << "Turn Right\n";
		return 0b0001;
	}
	std::cout << "No Marker Found\n";
	return 0b0000;
}

void
target_finder(std::vector<std::vector<cv::Point2f>>& marker_array, std::vector<std::vector<cv::Point2f>>& target_coord, std::vector<
	int> id_array, int id)
{
	for (int i = 0; i < id_array.size(); i++)
	{
		if (id_array[i] == id)
		{
			target_coord = { marker_array[i] };
		}
	}
}

std::vector<int> translate_found_markers(std::unique_ptr<dictionary::MarkerDict>& md, std::vector<int>& input_ids)
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

int main(int argc, char* argv[])
{
	overlay::DEBUG_FLAG = false;
//	bool target_flag =
	bool all_markers = false;
	bool visual_flag = false;
//	bool location_flag = true;

//	bool drive_flag = false;

	std::shared_ptr<cv::Mat> frame = std::make_shared<cv::Mat>(cv::Mat::zeros(cv::Size(overlay::WINDOW_WIDTH, overlay::WINDOW_HEIGHT), CV_8UC3));

	for (int i = 1; i < argc; i++)
	{
		if (std::strcmp(argv[i], "--debug") == 0 || std::strcmp(argv[i], "-d") == 0)
		{
			overlay::DEBUG_FLAG = true;
		}
		if (std::strcmp(argv[i], "--all") == 0 || std::strcmp(argv[i], "-a") == 0)
		{
			all_markers = true;
		}
		if (std::strcmp(argv[i], "--visual") == 0 || std::strcmp(argv[i], "-v") == 0)
		{
			visual_flag = true;
		}
	}
	overlay::ColumnOverlay c_o(200);

	std::unique_ptr<dictionary::MarkerDict> md(new dictionary::MarkerDict("marker_dict"));

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

//	std::cout << "Press any Key to Start\n";

	// -- Main Loop --
	std::cout << "Press any key to terminate\n";
	for (;;)
	{
		int key_press = cv::waitKey(5);
		uchar direction = 0b0000;
//		std::shared_ptr<cv::Mat> frame;
		cap.read(*frame);
		// -- validate frame --
		if (frame->empty())
		{
			std::cerr << "Error : Blank Frame Grabbed\n";
			break;
		}

		std::vector<int> marker_ids;
		std::vector<std::vector<cv::Point2f>> marker_corners, rejected_candidates;

		detector.detectMarkers(*frame, marker_corners, marker_ids, rejected_candidates);

		// -- KEEP THIS PART FOR CORNERS DEBUG -
		if (!marker_ids.empty() && all_markers)
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
		// -------------------------------------

		std::vector<int> marker_states = translate_found_markers(md, marker_ids);

		// -- TARGET COORD -- FUNCTION?
		if (!marker_ids.empty())
		{
			std::vector<std::vector<cv::Point2f>> t_coord;
			target_finder(marker_corners, t_coord, marker_ids, id);
			if (!t_coord.empty() && overlay::DEBUG_FLAG)
			{
				std::cout << "Marker id=" << id << "{\n";
				for (auto entry : t_coord[0])
				{
					std::cout << entry << "\n";
				}
				std::cout << "\n}\n";
			}
			else if (overlay::DEBUG_FLAG)
			{
				std::cout << "Field Empty\n";

			}

			if (!t_coord.empty())
			{
				direction = steering(t_coord[0], { id }, c_o);
				std::cout << id << " :" << (int)direction << "=" << direction_map[direction] << "\n";
				cv::aruco::drawDetectedMarkers(*frame, marker_corners, t_coord);
			}

		}

		// -- KEEP --
		if (all_markers)
		{
			for (int i = 0; i < marker_ids.size(); i++)
			{
				std::cout << "\nid = " << marker_ids[i] << "\nColumn : " << c_o.within_bounds(marker_corners[i])
						  << "\nPos : " << overlay::position_translation[c_o.marker_position(marker_corners[i])];
			}
		}

		// -- Send Frame At this point --
		// #######################
		// Send Directions -- direciton variable
		// Send Frame frame::data()
		uchar* point = frame->data;
//		std::cout << (int)*point << "\n";

		if (visual_flag)
		{
			if (!marker_ids.empty())
			{
				cv::aruco::drawDetectedMarkers(*frame, marker_corners, marker_ids);
			}
			cv::imshow("Live", *frame);
		}
		if (key_press >= 0)
		{
			break;
		}
	}

	return 0;
}
