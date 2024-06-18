//
// Created by maxwe on 25/05/24.
//

#include "column_overlay.hpp"
#include "marker_dict.hpp"
#include "square_overlay.hpp"
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

std::map<uchar, std::string> direction_map = { std::pair<uchar, std::string>(0b0000, "Stop"),
	std::pair<uchar, std::string>(0b0001, "T_Right"), std::pair<uchar, std::string>(0b0010, "MV_Forward"),
	std::pair<uchar, std::string>(0b0100, "T_Left"), std::pair<uchar, std::string>(0b1111, "Invalid"),

};

uchar steering(std::vector<cv::Point2f> target_marker, std::vector<int>* id, overlay::ColumnOverlay col_overlay)
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

std::vector<std::string>
translate_found_markers(std::shared_ptr<dictionary::MarkerDict>& md, std::vector<int>& input_ids)
{
	std::vector<std::string> marker_states;
	for (int input_id : input_ids)
	{
		marker_states.push_back(md->marker_translate(input_id));
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

void
draw_objects(cv::Mat& frame, std::shared_ptr<dictionary::MarkerDict> md, std::vector<int> marker_ids, std::vector<std::vector<
	cv::Point2f>> marker_vertices)
{
	std::map<int, std::string> md_local = std::map<int, std::string>(md->return_dict());
	for (int i = 0; i < marker_ids.size(); i++)
	{
		int id = marker_ids[i];
		std::vector<cv::Point2f> marker = std::vector<cv::Point2f>(marker_vertices[i]);
		std::string obj = md_local[id];
		cv::putText(frame,
			obj,
			marker[3],
			cv::FONT_HERSHEY_DUPLEX, 0.5, CV_RGB(0xfe, 0x70, 0xc6), //font color
			1);

	}
}

int main(int argc, char* argv[])
{

	overlay::DEBUG_FLAG = false;
	bool target_flag = false;
	bool ids_flag = false;
	bool location_flag = false;
	bool drive_flag = false;
	bool no_visual = false;
	for (int i = 1; i < argc; i++)
	{
		if (std::strcmp(argv[i], "--debug") == 0 || std::strcmp(argv[i], "-d") == 0)
		{
			overlay::DEBUG_FLAG = true;
		}
		if (std::strcmp(argv[i], "--no-visual") == 0 || std::strcmp(argv[i], "-n") == 0)
		{
			no_visual = true;
			target_flag = true;
		}
	}
	cv::Mat frame;

	overlay::SquareOverlay sq_o(200);

	overlay::ColumnOverlay c_o =  overlay::ColumnOverlay(200);

	if (overlay::DEBUG_FLAG) std::cout << "Square Overlay Initialised\n";
	std::shared_ptr<dictionary::MarkerDict> md(new dictionary::MarkerDict("marker_dict"));

	std::vector<int> possible_ids = md->GetPossibleIds();


	// -- verify marker dict has been loaded properly --
	if (md->size_of_map() <= 1)
	{
		std::cerr << "Error : marker_dict no initialised properly\n";
		return 1;
	}
	;
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
	int id = possible_ids[0];
	int id_ = 1;
	// -- Main Loop --
	std::cout << "Press any key to terminate\n";
	for (;;)
	{
		id = possible_ids[id_];
		std::vector<int> t_id = std::vector<int>({ id });

		uchar* direction = new uchar(0b0000);

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

		std::vector<std::string> marker_states = translate_found_markers(md, marker_ids);

		// Draw Polygon Boundaries
//		sq_o.draw(frame);
		c_o.draw(frame);

		std::vector<std::vector<cv::Point2f>>* t_coord = nullptr;

		if (!marker_ids.empty())
		{
			for (int j = 0; j < marker_ids.size(); j++)
			{
				if (marker_ids[j] == id)
				{
					std::vector<cv::Point2f> t_corners_ = std::vector<cv::Point2f>(marker_corners[j]);
					t_coord = new std::vector<std::vector<cv::Point2f>>({ t_corners_ });


					if (overlay::DEBUG_FLAG) std::cout << "Found Target " << id << "\n";
				}
			}
		}

//		if (overlay::DEBUG_FLAG)
//		{
//			for (int i = 0; i < marker_ids.size(); i++)
//			{
//				std::cout << "\nid = " << marker_ids[i] << "\nSquare : " << sq_o.within_bounds(marker_corners[i])
//						  << "\nColumn : " << c_o.within_bounds(marker_corners[i]) << "\nPos : "
//						  << overlay::position_translation[c_o.marker_position(marker_corners[i])];
//			}
//		}
//
//		if (location_flag)
//		{
//			if (target_flag)
//			{
//				marker_corners = t_coord;
//				marker_ids = { id };
//
//			}
//			if (!marker_corners.empty())
//			{
//				for (int i = 0; i < marker_ids.size(); i++)
//				{
//					std::cout << i << ") ";
//					std::cout << "id=" << marker_ids[i] << " pos : "
//							  << overlay::position_translation[c_o.marker_position(marker_corners[i])] << "\n";
//				}
//			}
//		}
//		if (target_flag)
//		{
//			if (!marker_ids.empty())
//			{
//				std::vector<std::vector<cv::Point2f>> t_corner = t_coord;
//				std::vector<int> t_id = { id };
////				std::vector<int> t_state = translate_found_markers(md, t_id);
//				if (!t_corner.empty() && !t_id.empty())
//				{
//					std::vector<cv::Point2f> marker_coord = t_coord[0];
//					std::string state_string = md->marker_translate(t_id[0]);
//
//					if (ids_flag) cv::aruco::drawDetectedMarkers(frame, t_corner, t_id, cv::Scalar(0, 0, 0xff));
//					else {
//						draw_objects(frame, md, marker_ids, marker_corners);
//					}
//					direction = steering(t_corner[0], t_id, c_o);
//
////					cv::putText(frame, //target image
////						direction_map[direction], //text
////						marker_coord[3], //top-left position
////						cv::FONT_HERSHEY_DUPLEX, 0.5, CV_RGB(0, 255, 0), //font color
////						1);
//
//
//
//				}
//			}
//		}
//		else
//		{
//			if (ids_flag) cv::aruco::drawDetectedMarkers(frame, marker_corners, marker_ids, cv::Scalar(0, 0, 0xff));
//			else cv::aruco::drawDetectedMarkers(frame, marker_corners, marker_states);
//		}
		// ------- draw methods --------
		if (!target_flag && (!marker_ids.empty() || marker_ids.size() != marker_corners.size()))
		{
			cv::aruco::drawDetectedMarkers(frame, marker_corners, marker_ids, cv::Scalar(0xFF, 0x00, 0x7F));
			draw_objects(frame, md, marker_ids, marker_corners);
		}
		else if (t_coord != nullptr)
		{
			if (target_flag && (!t_id.empty() || t_id.size() != t_coord->size()))
			{

				cv::aruco::drawDetectedMarkers(frame, *t_coord, t_id, cv::Scalar(0xFF, 0x00, 0x7F));

				draw_objects(frame, md, t_id, *t_coord);
				*direction = steering((*t_coord)[0], &t_id, c_o);
//				delete t_id_;
				std::cout << t_id[0] << " : " << direction_map[(int)(*direction)] << "\n";
			}
		}

		if (drive_flag) std::cout << direction_map[*direction] << "\n";
		if(!no_visual)
		{
			cv::imshow("Live", frame);
		}

//		delete t_coord;
		delete direction;
//		delete t_id;
//		delete c_o;
		// --  close window when key pressed --
		if (key_press == (int)'i')
		{
			std::cout << "Toggle ID/METHOD\n";
			ids_flag = !ids_flag;
		}
		else if (key_press == (int)'d')
		{
			std::cout << "Toggle Debug\n";
			overlay::DEBUG_FLAG = !overlay::DEBUG_FLAG;
		}
		else if (key_press == (int)'c')
		{
			std::cout << "Toggle Drive\n";
			drive_flag = !drive_flag;
		}
		else if (key_press == (int)'l')
		{
			std::cout << "Toggle Locations (l/r/m)\n";
			if (id_ == possible_ids.size()-1)
			{
				id_=0;
			}
			id_++;

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
	};

	return 0;
}
