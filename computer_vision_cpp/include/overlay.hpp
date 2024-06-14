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
		std::pair<uchar, std::string>(0b0000, std::string("Invalid")),
		std::pair<uchar, std::string>(0b0100, std::string("Left")),
		std::pair<uchar, std::string>(0b0110, std::string("Left-Middle")),
		std::pair<uchar, std::string>(0b0010, std::string("Middle")),
		std::pair<uchar, std::string>(0b0011, std::string("Right-Middle")),
		std::pair<uchar, std::string>(0b0001, std::string("Right")),
		std::pair<uchar, std::string>(0b0111, std::string("Invalid")), };

	class Overlay
	{
	 protected:
		int window_height;
		int window_width;
	 public:
		Overlay();

		virtual ~Overlay();

		[[maybe_unused]] virtual bool within_bounds();

		[[maybe_unused]] virtual bool point_in_bounds();

		[[nodiscard]] virtual std::string print() const;

		[[maybe_unused]] virtual void draw();

		[[maybe_unused]] virtual uchar position();
	};

	std::ostream& operator<<(std::ostream& os, Overlay const& o);

}
#endif //EE2_COMPUTER_VISION_OVERLAY_HPP
