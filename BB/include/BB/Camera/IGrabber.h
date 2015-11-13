/*
 * IGrabber.h
 *
 *  Created on: 1. 11. 2013
 *      Author: dron
 */

#ifndef IGRABBER_H_
#define IGRABBER_H_

#include <Poco/SharedPtr.h>
#include <opencv2/core/core.hpp>
#include <Poco/BasicEvent.h>


namespace BB {
	namespace Camera {

		class IGrabber {
			public:
				typedef Poco::SharedPtr<IGrabber> Ptr;


				virtual ~IGrabber() {}

			    virtual bool open(const std::string& filename) = 0;
			    virtual bool open(int device) = 0;
			    virtual bool isOpened() const = 0;

			    virtual IGrabber& operator >> (cv::Mat& image) = 0;
			    virtual bool read(cv::Mat& image) = 0;

			    virtual void release() = 0;

			    Poco::BasicEvent<bool> StreamStopped;
		};

	} /* namespace Camera */
} /* namespace TBS */

#endif /* IGRABBER_H_ */
