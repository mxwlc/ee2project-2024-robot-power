#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <opencv2/objdetect/aruco_detector.hpp>
#include <opencv2/opencv.hpp>


/*
This needs changing to add for more "jobs"
*/
typedef enum
{
    STOP = 0,
    FORWARD = 1,
    BACKWARD = 2,
    TURN_L = 3,
    TURN_R = 4,
    STATES_NR_ITEMS = 5
} states;



class marker_dict
{
private:

    // internal MarkID -> State mapping
    std::map<int, states> marker_map;

    // maps State to the corresponding label
    std::map<states, std::string> enum_to_string = {{states::STOP, "STOP"},
                                                {states::FORWARD, "FORWARD"},
                                                {states::BACKWARD, "BACKWARD"},
                                                {states::TURN_L, "TURN_L"},
                                                {states::TURN_R, "TURN_R"},
                                                {states::STATES_NR_ITEMS, std::to_string(int(states::STATES_NR_ITEMS))}};

public:
    // Constructors
    marker_dict();
    marker_dict(std::map<int, states> &dict);
    marker_dict(std::string filename);

    // Destructor
    ~marker_dict();

    // Add {marker_id, state} pair to the internal marker map
    void add_marker(int id, states marker_state);

    // returns the state for a given id
    states marker_translate(int id);

    // debug : output entire map
    std::map<int, states> return_dict() const;

    // debug : prints the marker map hash
    std::string print_dict() const;

    // Stores the internal marker_map locally
    void save_dict();

    // debug : translates state enums to a corresponding string
    std::string enum_string_translation(states in_state);

    // returns the size of the internal marker map
    int size_of_map();

    // Read marker_map from a file
    std::map<int, states> load_marker_map(std::string filename);
};

// Cout class#
std::ostream &operator<<(std::ostream &os, marker_dict const &m);