//
// Created by maxwe on 08/06/24.
//
#include "../include/overlay.hpp"

namespace overlay {
    overlay::overlay()
    {
        window_height = WINDOW_HEIGHT;
        window_width = WINDOW_WIDTH;
    }

    overlay::~overlay() { }

    bool overlay::within_bounds()
    {
        return false;
    }

	std::string overlay::print() const
	{
		return {"Empty Overlay"};
	};

    void overlay::draw(){};

	bool overlay::point_in_bounds()
	{
		return false;
	}
	uchar overlay::position(){
		return 0b111;
	}

	std::ostream& operator<<(std::ostream& os, overlay const& o)
    {
		std::string output = o.print();
        return os << output;
    }
}