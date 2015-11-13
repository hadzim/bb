/*
 * OpenCVGrabber.cpp
 *
 *  Created on: 1. 11. 2013
 *      Author: dron
 */
#include <BB/Camera/OpenCVGrabber.h>


namespace BB {
	namespace Camera {

		OpenCVGrabber::OpenCVGrabber() {
		}

		OpenCVGrabber::~OpenCVGrabber() {

		}

		bool OpenCVGrabber::open(const std::string& filename) {
			return cap.open(filename);
		}
		bool OpenCVGrabber::open(int device) {
			return cap.open(device);
		}
		bool OpenCVGrabber::isOpened() const {
			return cap.isOpened();
		}

		OpenCVGrabber& OpenCVGrabber::operator >> (cv::Mat& image) {
			cap >> image;
			return *this;
		}

		bool OpenCVGrabber::read(cv::Mat& image) {
			return cap.read(image);
		}

		void OpenCVGrabber::release(){
			cap.release();
		}

	    //REGISTER_GRABBER_OBJECT("OpenCV", OpenCVGrabber, IGrabber, OpenCVGrabber_)

	} /* namespace Camera */
} /* namespace TBS */
