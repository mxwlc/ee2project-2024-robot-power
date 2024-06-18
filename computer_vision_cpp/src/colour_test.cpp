//
// Created by maxwe on 14/06/24.
//
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "../include/overlay.hpp"


int main(){
	cv::Mat empty = cv::Mat::zeros(cv::Size(overlay::WINDOW_WIDTH,overlay::WINDOW_HEIGHT), CV_8UC3);
	cv::Mat channels[3];
	cv::Mat blue, green, red;
	cv::Mat full = cv::Mat::ones(cv::Size(overlay::WINDOW_WIDTH,overlay::WINDOW_HEIGHT), CV_8UC3);
	cv::split(full, channels);
	cv::Mat blue_image[3] = {channels[0]*255, channels[1], channels[2]};
	cv::Mat green_image[3] = {channels[1], channels[0]*255, channels[2]};
	cv::Mat red_image[3]= {channels[1], channels[2], channels[0]*255};
	cv::merge(blue_image,3, blue);
	cv::merge(green_image, 3, green);
	cv::merge(red_image, 3, red);

	cv::Mat frame;
	int count = 0;
	for(;;){
		if(count <250){
			frame = empty;
		}
		if (count >= 250 && count < 500){
			frame = blue;
		}
		if(count>=500 && count < 750){
			frame = green;
		}
		if(count >=750){
			frame = red;
		}
		if(count==1000){
			count = 0;
		}
		std::cout << count << "\n";
		count++;
		cv::imshow("Live", frame);
		auto key_press = cv::waitKey(5);
		if(key_press>=0){
			break;
		}

	}
}
