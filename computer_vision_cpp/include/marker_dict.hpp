#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <opencv2/opencv.hpp>
// #include <opencv2/objdetect/aruco_detector.hpp>
// #include <boost/serialization/map.hpp>
// #include <boost/archive/text_oarchive.hpp>
// #include <boost/archive/text_iarchive.hpp>


// #pragma once

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

    // debug : output entire map
    std::map<int, states> return_dict();

    void print_dict();

    // void save_dict();
};
