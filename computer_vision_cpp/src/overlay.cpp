//
// Created by maxwe on 08/06/24.
//
#include "../include/overlay.hpp"

namespace overlay
{
	overlay::overlay()
	{
		window_height = WINDOW_HEIGHT;
		window_width = WINDOW_WIDTH;
	}

	overlay::~overlay()
	{
	}

	[[maybe_unused]] bool overlay::within_bounds()
	{
		return false;
	}

	std::string overlay::print() const
	{

		return { quote(overlay) };
	};

	[[maybe_unused]] void overlay::draw()
	{
	};

	[[maybe_unused]] bool overlay::point_in_bounds()
	{
		return false;
	}
	[[maybe_unused]] uchar overlay::position()
	{
		return 0b111;
	}

	std::ostream& operator<<(std::ostream& os, overlay const& o)
	{
		std::string output = o.print();
		return os << output;
	}
}