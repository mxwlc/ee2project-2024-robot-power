//
// Created by maxwe on 27/05/24.
//
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <opencv2/objdetect/aruco_board.hpp>
#include <opencv2/objdetect/aruco_detector.hpp>
#include <opencv2/opencv.hpp>

// TODO : Save map to local file for persistance

#define MARKER_EDGE_SIZE  200
#define BORDER_SIZE  1
#define ARUCO_DICTIONARY cv::aruco::DICT_5X5_250
#define MAX_MARKER_NUMBER 250

typedef enum
{
    STOP = 0,
    FORWARD = 1,
    BACKWARD = 2,
    TURN_L = 3,
    TURN_R = 4
} states;

class marker_dict
{
private:
    std::map<int, states> marker_map;

public:
    // Constructors
    marker_dict();
    marker_dict(std::map<int, states> &dict);

    // Destructor
    ~marker_dict();

    // Add {marker_id, state} pair to the internal marker map
    void add_marker(int id, states marker_state);

    // returns the state for a given id
    states marker_translate(int id);

    // debug : entire map
    void print_dict();
};

marker_dict::marker_dict()
{
    std::cout << "Marker Dictionary Initialisation Start" << std::endl;
    std::map<int, states> temp_map({{0, states::STOP}});
    marker_map = temp_map;
    std::cout << "Marker Dictionary Initialisation End" << std::endl;
}

marker_dict::marker_dict(std::map<int, states> &dict)
{
    marker_map = dict;
}

marker_dict::~marker_dict()
{
    std::cout << "Marker Dictionary Leaves Scope" << std::endl;
}

void marker_dict::add_marker(int id, states marker_state)
{
    marker_map.insert({id, marker_state});
}

states marker_dict::marker_translate(int id)
{
    return marker_map[id];
}

void marker_dict::print_dict()
{
    std::cout << "Key, Value " << std::endl;
    for (auto &t : marker_map)
    {
        std::cout << t.first << " " << states(t.second) << std::endl;
    }
}


std::vector<uchar> id_array()
{
    std::cout << "-------------------------------------------------------------------------------" << std::endl;
    bool valid = false;
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
        valid = false;
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


void add_to_dictionary(marker_dict &dict, int id)
{
    int choice;
    bool valid = false;
    do
    {
        std::cout << "What Should Marker (id =" << id << ") Do" << std::endl;
        std::cout << "-------------------------------------------------------------------------------" << std::endl
                  << "(0) : STOP" << std::endl
                  << "(1) : FORWARD" << std::endl
                  << "(2) : BACKWARDS" << std::endl
                  << "(3) : TURN_L (turn left)" << std::endl
                  << "(4) : TURN_R (turn right)" << std::endl
                  << "-------------------------------------------------------------------------------" << std::endl;
        std::cin >> choice;
        if (std::cin.fail())
        {
            std::cout << std::endl << "Please Enter a Valid Number" << std::endl;
            valid = false;
        }
        else if (choice > 4 || choice < 0)
        {
            std::cout << std::endl << "Please Enter A valid choice (0-4)" << std::endl;
            valid = false;
        }
        else
        {
            valid = true;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while (!valid);
    states curr_state;
    switch (choice)
    {
    case 0:
        curr_state = states::STOP;
        break;
    case 1:
        curr_state = states::FORWARD;
        break;
    case 2:
        curr_state = states::BACKWARD;
        break;
    case 3:
        curr_state = states::TURN_L;
        break;
    case 4:
        curr_state = states::TURN_R;
        break;
    default:
        curr_state = states::STOP;
        break;
    }
    dict.add_marker(id, curr_state);
}


int main()
{
    std::cout << "-------------------------------------------------------------------------------" << std::endl
              << "Marker Generator" << std::endl
              << "-------------------------------------------------------------------------------" << std::endl;
    std::shared_ptr<marker_dict> md(new marker_dict());

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

    for (int i = 0; i < curr_id_array.size(); i++)
    {

        cv::Mat marker;
        uchar id = curr_id_array[i];
        std::cout << "Generating Marker With id = " << int(curr_id_array[i]) << std::endl;
        cv::aruco::generateImageMarker(dict, curr_id_array[i], MARKER_EDGE_SIZE, marker, BORDER_SIZE);
        std::string marker_filename = "markers/marker_id" + std::to_string(int(id)) + ".bmp";
        cv::imwrite(marker_filename, marker);
        std::cout << "-------------------------------------------------------------------------------" << std::endl;
        std::cout << "Starting state selection logic" << std::endl;
        add_to_dictionary(*md, id);
        std::cout << "-------------------------------------------------------------------------------" << std::endl;
    }

    md->print_dict();
    return 0;
}
