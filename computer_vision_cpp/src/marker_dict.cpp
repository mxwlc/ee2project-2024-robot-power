#include "../include/marker_dict.hpp"
// TODO write comments and documentation

marker_dict::marker_dict()
{
    /*
     * Default constructor, initialises the map with a {0, STOP} pair
     */
    std::cout << "Marker Dictionary Initialisation Start" << std::endl;
    std::map<int, states> temp_map({{0, states::STOP}});
    marker_map = temp_map;
    std::cout << "Marker Dictionary Initialisation End" << std::endl;
}

marker_dict::marker_dict(std::map<int, states> &dict)
{
    /*
     * Constructor setting the internal map to the provided arguement map
     */
    marker_map = dict;
}

marker_dict::marker_dict(std::string filename)
{
    /*
     * Constructor, setting marker map to the map stored in the provided text file
     */
    std::map<int, states> t_marker_map;
    marker_map = load_marker_map(filename);
}

marker_dict::~marker_dict()
{
    // Debug atm
    // Destructor that tells the user when the marker dict object (and thus the shared_ptr to the object) leaves the current scope (or the ptr is deleted)
    std::cout << "Marker Dictionary Leaves Scope" << std::endl;
}

void marker_dict::add_marker(int id, states marker_state)
{
    /*
     * Adds a {marker_id, state} Pair to the internal map
     */
    marker_map.insert({id, marker_state});
}

states marker_dict::marker_translate(int id)
{
    /*
     * Returns the corresponding enum State from the provided marker id
     */
    return marker_map[id];
}

std::map<int, states> marker_dict::return_dict() const
{
    /*
     * Returns the internal marker map object
     */
    return marker_map;
}

std::string marker_dict::print_dict() const
{
    /*
     * Debug print
     * May haps be useless
     */
    std::string output;
    std::string line;
    output = std::string("ID, State \n") + std::string("{\n");
    for (auto &t : marker_map)
    {
        line = std::to_string(t.first) +  std::string(" : ") + std::to_string(states(t.second)) + std::string("\n");
        output.append(line);
    }
    output.append("}\n");
    return output;
}

std::ostream &operator<<(std::ostream &os, marker_dict const &m)
{
    std::string output = m.print_dict();
    return os << output;
}

void marker_dict::save_dict()
{
    std::string filename("marker_dict");
    std::ofstream outfile(filename);

    if (!outfile.is_open())
    {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }
    for (const auto &pair : marker_map)
    {
        outfile << pair.first << " " << pair.second << std::endl;
    }

    outfile.close();

    if (!outfile.good())
    {
        std::cerr << "Error occurred at writing time!" << std::endl;
    }
}

int marker_dict::size_of_map()
{
    return marker_map.size();
}

std::map<int, states> marker_dict::load_marker_map(std::string filename)
{
    std::ifstream input_file(filename);
    std::map<int, states> marker_map;
    if (!input_file)
    {
        std::cerr << "Error : Cannot open file\n";
        marker_map.insert({{0, states::STOP}});
        return marker_map;
    }
    std::string key, value;
    while (input_file >> key >> value)
    {
        marker_map[std::stoi(key)] = (states)std::stoi(value);
    }
    return marker_map;
}
std::string marker_dict::enum_string_translation(states in_state){
    return enum_to_string[in_state];
}
