//
// Created by maxwe on 27/05/24.
//
#include <cmath>
#include <iostream>
#include <map>
#include <opencv2/objdetect/aruco_board.hpp>
#include <opencv2/objdetect/aruco_detector.hpp>
#include <opencv2/opencv.hpp>
#include "../include/marker_dict.hpp"

#define MARKER_EDGE_SIZE  200
#define BORDER_SIZE  1
#define ARUCO_DICTIONARY cv::aruco::DICT_5X5_250
#define MAX_MARKER_NUMBER 250

std::vector<uchar> id_array()
{

	std::cout << "-------------------------------------------------------------------------------" << std::endl;
	bool valid;
	int total_marker;
	do
	{
		std::cout << "Please insert the total Number of Markers You would Like to generate (1-10): ";
		std::cin >> total_marker;
		if (std::cin.fail())
		{
			std::cout << std::endl << "Please Enter a Valid Number" << std::endl;
			valid = false;
		}
		else if (total_marker > 10 || total_marker < 1)
		{
			std::cout << std::endl << "Please Enter A number within the range (1-10)" << std::endl;
			valid = false;
		}
		else
		{
			valid = true;
		}
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	} while (!valid);

	std::cout << "-------------------------------------------------------------------------------" << std::endl
			  << "You are generating " << total_marker << " marker(s)" << std::endl
			  << "-------------------------------------------------------------------------------" << std::endl;
	std::vector<uchar> marker_ids;

	for (int i = 0; i < total_marker; i++)
	{

		int current_id;
		do
		{
			std::cout << "Please insert the ID for marker " << (i + 1) << std::endl;
			std::cin >> current_id;
			if (std::cin.fail())
			{
				std::cout << std::endl << "Please Enter a Valid Number" << std::endl;
				valid = false;
			}
			else if (total_marker > MAX_MARKER_NUMBER || total_marker < 0)
			{
				std::cout << std::endl << "Please Enter A number within the range (0-255)" << std::endl;
				valid = false;
			}
			else
			{
				valid = true;
				marker_ids.push_back(current_id & 0xff);
			}
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		} while (!valid);
	}
	return marker_ids;
}

void add_to_dictionary(dictionary::MarkerDict& dict, int id)
{
/* Needs changing to support a variable enum size
 *
 * */
	std::string choice;
	bool valid;
	do
	{
		std::cout << "What Should Marker (id =" << id << ") Do" << std::endl;
		std::cout << "-------------------------------------------------------------------------------" << std::endl;
		// Print Menu
//		for (int i = 0; i < (int)dictionary::states::STATES_NR_ITEMS; i++)
//		{
//			std::cout << i << " : " << dict.enum_string_translation(static_cast<dictionary::states>(i)) << "\n";
//		}

//		std::cout << "-------------------------------------------------------------------------------" << std::endl;
		std::cin >> choice;
		if (std::cin.fail())
		{
			std::cout << std::endl << "Please Enter a Valid Number" << std::endl;
			valid = false;
		}
//		else if (choice >= (int)dictionary::STATES_NR_ITEMS || choice < 0)
//		{
//			std::cout << std::endl << "Please Enter A valid choice (0-" << (int)dictionary::STATES_NR_ITEMS << ")\n";
//			valid = false;
//		}
		else
		{
			valid = true;
		}
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	} while (!valid);
//	dictionary::states curr_state;
//	switch (choice)
//	{
//	case 0:
//		curr_state = dictionary::states::STOP;
//		break;
//	case 1:
//		curr_state = dictionary::states::GO_TOWARDS;
//		break;
//	case 2:
//		curr_state = dictionary::states::GO_AWAY_FROM;
//		break;
//	default:
//		curr_state = dictionary::states::STOP;
//		break;
//	}
	dict.add_marker(id, choice);
}

int main()
{
	std::cout << "-------------------------------------------------------------------------------" << std::endl
			  << "Marker Generator" << std::endl
			  << "-------------------------------------------------------------------------------" << std::endl;
	std::shared_ptr<dictionary::MarkerDict> md(new dictionary::MarkerDict());

	std::cout << "-------------------------------------------------------------------------------" << std::endl
			  << "Loading Predefined Dictionary" << std::endl
			  << "-------------------------------------------------------------------------------" << std::endl;
	cv::aruco::Dictionary dict = cv::aruco::getPredefinedDictionary(ARUCO_DICTIONARY);

	std::cout << "-------------------------------------------------------------------------------" << std::endl;
	std::cout << "Dictionary Initiated at memory address " << &md << std::endl;
	std::cout << "-------------------------------------------------------------------------------" << std::endl;

	std::vector<uchar> curr_id_array = id_array();
	std::cout << "-------------------------------------------------------------------------------" << std::endl
			  << "Id Array Size : " << curr_id_array.size() << std::endl
			  << "-------------------------------------------------------------------------------" << std::endl;

	for (unsigned char i : curr_id_array)
	{

		cv::Mat marker;
		uchar id = i;
		std::cout << "Generating Marker With id = " << int(i) << std::endl;
		cv::aruco::generateImageMarker(dict, i, MARKER_EDGE_SIZE, marker, BORDER_SIZE);
		std::string marker_filename = "markers/marker_id" + std::to_string(int(id)) + ".bmp";
		cv::imwrite(marker_filename, marker);
		std::cout << "-------------------------------------------------------------------------------" << std::endl;
		std::cout << "Starting state selection logic" << std::endl;
		add_to_dictionary(*md, id);
		std::cout << "-------------------------------------------------------------------------------" << std::endl;
	}

	std::cout << *md << std::endl;
	md->save_dict();
	return 0;
}
