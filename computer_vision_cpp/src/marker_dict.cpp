#include "../include/marker_dict.hpp"

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

std::map<int, states> marker_dict::return_dict()
{
    return marker_map;
}

void marker_dict::print_dict()
{
    std::cout << "ID, State \n" << "{\n";
    for (auto &t : marker_map)
    {
        std::cout << t.first << " : " << states(t.second) << "\n";
    }
    std::cout << "}\n";
}

void marker_dict::save_dict()
{
    std::string filename("marker_dict");
    std::ofstream outfile(filename);

    if (!outfile.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }
    for (const auto& pair : marker_map) {
        outfile << pair.first << " " << pair.second << std::endl;
    }

    outfile.close();

    if (!outfile.good()) {
        std::cerr << "Error occurred at writing time!" << std::endl;
    }
}
