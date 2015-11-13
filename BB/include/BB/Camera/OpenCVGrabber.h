/*
 * OpenCVGrabber.h
 *
 *  Created on: 1. 11. 2013
 *      Author: dron
 */

#ifndef OPENCVGRABBER_H_
#define OPENCVGRABBER_H_

#include "IGrabber.h"
#include <opencv2/highgui/highgui.hpp>

namespace BB {
	namespace Camera {

		class OpenCVGrabber: public IGrabber {
			public:
				OpenCVGrabber();
				virtual ~OpenCVGrabber();

				virtual bool open(const std::string& filename);
				virtual bool open(int device);
				virtual bool isOpened() const;

				virtual OpenCVGrabber& operator >> (cv::Mat& image);
				virtual bool read(cv::Mat& image);

				virtual void release();
			private:
				cv::VideoCapture cap;

		};

	} /* namespace Camera */
} /* namespace TBS */

#endif /* OPENCVGRABBER_H_ */
