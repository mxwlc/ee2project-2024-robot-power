

#ifndef EE2_COMPUTER_VISION_MARKER_DICT_HPP
#define EE2_COMPUTER_VISION_MARKER_DICT_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <opencv2/objdetect/aruco_detector.hpp>
#include <opencv2/opencv.hpp>

#pragma once
namespace dictionary
{
/*
This needs changing to add for more "jobs"
*/
	typedef enum
	{
		STOP = 0, GO_TOWARDS = 1, GO_AWAY_FROM = 2, STATES_NR_ITEMS = 3
	} states;

	static std::map<states, std::string>  enum_to_string  = {{ states::STOP, "STOP" }, { states::GO_TOWARDS, "GO TOWARDS" },
		{ states::GO_AWAY_FROM, "GO AWAY FROM" },
		{ states::STATES_NR_ITEMS, std::to_string(int(states::STATES_NR_ITEMS)) }} ;

	class MarkerDict
	{
	 private:

		// internal MarkID -> State mapping
		std::map<int, std::string> marker_map;

		std::vector<int> possible_ids;
		// maps State to the corresponding label

	 public:
		// Constructors
		MarkerDict();

		explicit MarkerDict(std::map<int, std::string>& dict);

		explicit MarkerDict(std::string filename);

		// Destructor
		~MarkerDict();

		// Add {marker_id, state} pair to the internal marker map
		void add_marker(int id, std::string marker_state);

		// returns the state for a given id
		std::string marker_translate(int id);

		// debug : output entire map
		[[nodiscard]] std::map<int, std::string > return_dict() const;

		// debug : prints the marker map hash
		[[nodiscard]] std::string print_dict() const;

		// Stores the internal marker_map locally
		void save_dict();

		// returns the size of the internal marker map
		int size_of_map();

		// Read marker_map from a file
		std::map<int, std::string> load_marker_map(std::string filename);

		const std::vector<int>& GetPossibleIds() const;

		void SetPossibleIds(const std::vector<int>& possibleIds);

	};

// Cout class#
	std::ostream& operator<<(std::ostream& os, MarkerDict const& m);
}

#endif //EE2_COMPUTER_VISION_MARKER_DICT_HPP