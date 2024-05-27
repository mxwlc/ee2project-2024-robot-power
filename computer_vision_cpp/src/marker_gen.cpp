//
// Created by maxwe on 27/05/24.
//
#include <cmath>
#include <iostream>
#include <map>
#include <opencv2/objdetect/aruco_board.hpp>
#include <opencv2/opencv.hpp>

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
};

marker_dict::marker_dict()
{
    std::cout << "Marker Dictionary Initialisation Start" << std::endl;
    std::map<int, states> temp_map({{0, states::STOP}});
    // temp_map->insert({0, states::STOP});
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

std::vector<uchar> id_array()
{
    std::cout << "-------------------------------------------------------------------------------" << std::endl
              << "Please insert the total Number of Markers You would Like to generate : ";
    uchar marker_number;


}

int main()
{
    std::cout << "-------------------------------------------------------------------------------" << std::endl
              << "Marker Generator" << std::endl
              << "-------------------------------------------------------------------------------" << std::endl;
    std::shared_ptr<marker_dict> md(new marker_dict());
    std::cout << "-------------------------------------------------------------------------------" << std::endl;
    std::cout << "Dictionary Initiated at memory address " << &md << std::endl;


    md->add_marker(10, states::STOP);
    std::cout << md->marker_translate(10) << std::endl;
    uchar marker_id[] = {10, 20, 30, 40, 50};
    cv::Mat marker;
    cv::aruco::Dictionary dict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_5X5_250);
    std::cout << marker_id << " is of size " << sizeof(marker_id) << std::endl;
    for (int i = 0; i < sizeof(marker_id); ++i)
    {
        std::cout << "Marker ID = " << i << std::endl;
        cv::aruco::generateImageMarker(dict, marker_id[i], 200, marker, 1);
        std::string marker_filename = "/markers/marker_id" + std::to_string(marker_id[i]) + ".bmp";
        cv::imwrite(marker_filename, marker);
    }
    return 0;
}
