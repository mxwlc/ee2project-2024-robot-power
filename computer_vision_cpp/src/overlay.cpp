//
// Created by maxwe on 08/06/24.
//
#include "../include/overlay.hpp"

namespace overlay
{
	Overlay::Overlay()
	{
		window_height = WINDOW_HEIGHT;
		window_width = WINDOW_WIDTH;
	}

	Overlay::~Overlay()
	{
	}

	[[maybe_unused]] bool Overlay::within_bounds()
	{
		return false;
	}

	std::string Overlay::print() const
	{

		return { quote(Overlay) };
	};

	[[maybe_unused]] void Overlay::draw()
	{
	};

	[[maybe_unused]] bool Overlay::point_in_bounds()
	{
		return false;
	}
	[[maybe_unused]] uchar Overlay::position()
	{
		return 0b111;
	}

	std::ostream& operator<<(std::ostream& os, Overlay const& o)
	{
		std::string output = o.print();
		return os << output;
	}
}