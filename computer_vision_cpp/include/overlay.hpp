//
// Created by maxwe on 08/06/24.
//
#ifndef EE2_COMPUTER_VISION_OVERLAY_HPP
#define EE2_COMPUTER_VISION_OVERLAY_HPP

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <map>

#define quote(x) #x

namespace overlay
{

	static int WINDOW_HEIGHT = 480;
	static int WINDOW_WIDTH = 640;
	static bool DEBUG_FLAG;

	static std::map<uchar, std::string> position_translation = {
		std::pair<uchar, std::string>(0b000, std::string("Invalid")),
		std::pair<uchar, std::string>(0b100, std::string("Left")),
		std::pair<uchar, std::string>(0b110, std::string("Left-Middle")),
		std::pair<uchar, std::string>(0b010, std::string("Middle")),
		std::pair<uchar, std::string>(0b011, std::string("Right-Middle")),
		std::pair<uchar, std::string>(0b001, std::string("Right")),
		std::pair<uchar, std::string>(0b111, std::string("Invalid")), };

	class overlay
	{
	 protected:
		int window_height;
		int window_width;
	 public:
		overlay();

		virtual ~overlay();

		virtual bool within_bounds();

		virtual bool point_in_bounds();

		virtual std::string print() const;

		virtual void draw();

		virtual uchar position();
	};

	std::ostream& operator<<(std::ostream& os, overlay const& o);

}
#endif //EE2_COMPUTER_VISION_OVERLAY_HPP
