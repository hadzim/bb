/*
 * LogoPage.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: dron
 */

#include "LogoPage.h"
#include <opencv2/opencv.hpp>
#include <iostream>

namespace BB {

	cv::Mat halftone(cv::Mat original){
		cv::Mat gray;
		cv::cvtColor(original,gray,CV_BGR2GRAY);

		uchar scale = 128; // change this parameter to 8, 32, 64, 128 to change the dot size
		for (int r=1; r<gray.rows-1; r++) {
		    for (int c=1; c<gray.cols-1; c++) {
		        uchar oldPixel = gray.at<uchar>(r,c);
		        uchar newPixel = oldPixel < 128 ? 0 : 255;
		        gray.at<uchar>(r,c) = newPixel;
		        int quantError = oldPixel - newPixel;
		        gray.at<uchar>(r+1,c)   +=  7./16. * quantError;
		        gray.at<uchar>(r-1,c+1) +=  3./16. * quantError;
		        gray.at<uchar>(r,c+1) +=  5./16. * quantError;
		        gray.at<uchar>(r+1,c+1) +=  1./16. * quantError;
		    }
		}
		return gray;
		//cv::imshow("halftone", gray);
		//cv::waitKey();
	}

	cv::Mat resizeToScreen(cv::Mat original, IScreen::Ptr screen){
		cv::Mat dst;
		cv::resize(original, dst, cv::Size(screen->width(), screen->height()));
		return dst;
	}

	void showOnScreen(cv::Mat image, IScreen::Ptr screen){
			int16_t w = image.cols;
			int16_t h = image.rows;

			for (int16_t y = 0; y < h; y++){
				for (int16_t x = 0; x < w; x++){
					//std::cout << x << " " << y << " is " << (int)image.at<uchar>(y,x) << std::endl;
					screen->drawPixel(x, y, (image.at<uchar>(y,x) > 128) ? 1 : 0);
				}
			}
	}


	LogoPage::LogoPage(IScreen::Ptr screen) : Page("logo", screen) {

	}

	LogoPage::~LogoPage() {

	}

	void LogoPage::show(){
		try {
		std::cout << "show logo" << std::endl;
		cv::Mat logo = cv::imread("logo.png");
		//cv::imshow("logo", logo);

		auto small = resizeToScreen(logo, screen);
		//cv::imshow("small", small);
		auto half = halftone(small);
		//cv::imshow("half", half);

		showOnScreen(half, screen);
		//cv::waitKey(10);
		} catch (...){
			std::cerr << "logo not shown - error" << std::endl;
		}

	}

} /* namespace BB */
