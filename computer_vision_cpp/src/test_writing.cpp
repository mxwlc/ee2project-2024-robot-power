//
// Created by maxwe on 06/06/24.
//
#include <iostream>
#include <opencv2/core.hpp>


int main (){
    cv::Mat m = cv::Mat::ones(10,10, CV_8UC3);

    m.setTo(cv::Scalar(1,1,1));
    std::cout << cv::format(m, cv::Formatter::FMT_NUMPY) << "\n";
    std::cout << m.size() << "\n";
    std::cout << m.total() * m.elemSize() * 8 << "\n";
    std::cout << sizeof(m) << "\n";
    std::cout << "Hello World \n";
}