#include "BB/Screen/OpenCvScreen.h"
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

namespace BB {
OpenCvScreen::OpenCvScreen(std::string name) :
		IScreen(OPENCVWIDTH, OPENCVHEIGHT), name(name) {

	intW = OPENCVWIDTH;
	intH = OPENCVHEIGHT;
	intW *= 3;
	intH *= 3;
	buffer = cv::Mat::zeros(cv::Size(intW, intH), CV_8UC1);
}


OpenCvScreen::~OpenCvScreen(){
}

// the most basic function, set a single pixel
void OpenCvScreen::drawPixel(int16_t x, int16_t y, uint16_t color) {
	if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height))
		return;


	int16_t t;
	switch (rotation) {
		case 1:
			t = x;
			x = y;
			y = OPENCVHEIGHT - 1 - t;
			break;
		case 2:
			x = OPENCVWIDTH - 1 - x;
			y = OPENCVHEIGHT - 1 - y;
			break;
		case 3:
			t = x;
			x = OPENCVHEIGHT - 1 - y;
			y = t;
			break;
	}

	if ((x < 0) || (x >= OPENCVWIDTH) || (y < 0) || (y >= OPENCVHEIGHT))
		return;

	int xx = x;
	xx = xx * 3;
	int yy = y;
	yy = yy * 3;

	//std::cout << "draw: " << x << " " << y << " is " << (int)(color > 0 ? 255 : 0) << std::endl;

	unsigned char c = color > 0 ? (2.5 * maxIntensity) : 0;

	buffer.at<unsigned char>(yy, xx) = c;
	buffer.at<unsigned char>(yy, xx+1) = c;
	buffer.at<unsigned char>(yy, xx+2) = c;
	buffer.at<unsigned char>(yy+1, xx) = c;
	buffer.at<unsigned char>(yy+1, xx+1) = c;
	buffer.at<unsigned char>(yy+1, xx+2) = c;
	buffer.at<unsigned char>(yy+2, xx) = c;
	buffer.at<unsigned char>(yy+2, xx+1) = c;
	buffer.at<unsigned char>(yy+2, xx+2) = c;
}

void OpenCvScreen::setMaxIntensity(int mi){
	maxIntensity = mi;
}

void OpenCvScreen::display() {
	cv::imshow(name, buffer);
	cv::waitKey(5);
}

// clear everything
void OpenCvScreen::clearDisplay(void) {
	buffer = cv::Mat::zeros(cv::Size(intW, intH), CV_8UC1);
	cursor_y = cursor_x = 0;
}

}
