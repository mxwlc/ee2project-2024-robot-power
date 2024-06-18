#include "../include/marker_dict.hpp"
// TODO write comments and documentation

namespace dictionary
{
	MarkerDict::MarkerDict()
	{
		/*
		 * Default constructor, initialises the map with a {0, STOP} pair
		 */
		std::cout << "Marker Dictionary Initialisation Start" << std::endl;
		std::map<int, std::string> temp_map({{ 0, std::string("STOP") }});
		possible_ids = {0};
		marker_map = temp_map;
		std::cout << "Marker Dictionary Initialisation End" << std::endl;
	}

	MarkerDict::MarkerDict(std::map<int, std::string >& dict)
	{
		/*
		 * Constructor setting the internal map to the provided arguement map
		 */
		marker_map = dict;
	}

	MarkerDict::MarkerDict(std::string filename)
	{
		/*
		 * Constructor, setting marker map to the map stored in the provided text file
		 */
		std::map<int, std::string> t_marker_map;
		marker_map = load_marker_map(filename);
	}

	MarkerDict::~MarkerDict()
	{
		// Debug atm
		// Destructor that tells the user when the marker dict object (and thus the shared_ptr to the object) leaves the current scope (or the ptr is deleted)
		std::cout << "Marker Dictionary Leaves Scope" << std::endl;
	}

	void MarkerDict::add_marker(int id, std::string marker_state)
	{
		/*
		 * Adds a {marker_id, state} Pair to the internal map
		 */
		marker_map.insert({ id, marker_state });
	}

	std::string MarkerDict::marker_translate(int id)
	{
		/*
		 * Returns the corresponding enum State from the provided marker id
		 */
		return marker_map[id];
	}

	std::map<int, std::string> MarkerDict::return_dict() const
	{
		/*
		 * Returns the internal marker map object
		 */
		return marker_map;
	}

	std::string MarkerDict::print_dict() const
	{
		/*
		 * Debug print
		 * May haps be useless
		 */
		std::string output;
		std::string line;
		output = std::string("ID, State \n") + std::string("{\n");
		for (auto& t : marker_map)
		{
			line = std::to_string(t.first) + std::string(" : ") + t.second + std::string("\n");
			output.append(line);
		}
		output.append("}\n");
		return output;
	}

	void MarkerDict::save_dict()
	{
		std::string filename("marker_dict");
		std::ofstream outfile(filename);

		if (!outfile.is_open())
		{
			std::cerr << "Failed to open file for writing: " << filename << std::endl;
			return;
		}
		for (const auto& pair : marker_map)
		{
			outfile << pair.first << " " << pair.second << std::endl;
		}

		outfile.close();

		if (!outfile.good())
		{
			std::cerr << "Error occurred at writing time!" << std::endl;
		}
	}

	int MarkerDict::size_of_map()
	{
		return (int)marker_map.size();
	}

	std::map<int, std::string> MarkerDict::load_marker_map(std::string filename)
	{
		std::ifstream input_file(filename);
		std::vector<int> possibleIds;
		std::map<int, std::string > marker_map_;
		if (!input_file)
		{
			std::cerr << "Error : Cannot open file\n";
			marker_map_.insert({{ 0, std::string("No Object")}});
			return marker_map_;
		}
		std::string key, value;
		while (input_file >> key >> value)
		{
			possibleIds.push_back(std::stoi(key));
			marker_map_[std::stoi(key)] = value;
		}
		possible_ids = possibleIds;
		return marker_map_;
	}
	const std::vector<int>& MarkerDict::GetPossibleIds() const
	{
		return possible_ids;
	}
	void MarkerDict::SetPossibleIds(const std::vector<int>& possibleIds)
	{
		possible_ids = possibleIds;
	}



	std::ostream& operator<<(std::ostream& os, MarkerDict const& m)
	{
		std::string output = m.print_dict();
		return os << output;
	}
}