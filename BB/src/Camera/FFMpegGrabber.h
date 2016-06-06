/*
 * FFMpegGrabber.h
 *
 *  Created on: May 23, 2016
 *      Author: dron
 */

#ifndef FFMPEGGRABBER_H_
#define FFMPEGGRABBER_H_
#include <BB/Camera/IGrabber.h>

#include "Poco/Runnable.h"
#include "Poco/Mutex.h"
#include "Poco/Thread.h"

#include <string>
#include "TBS/Nullable.h"

namespace BB {
	namespace Camera {

		class FFMpegGrabber : public IGrabber, public Poco::Runnable {
			public:
				FFMpegGrabber(std::string uniqueName);
				virtual ~FFMpegGrabber();

				virtual bool open(const std::string& filename);
				virtual bool open(int device);
				virtual bool isOpened() const;

				virtual IGrabber& operator >> (cv::Mat& image);
				virtual bool read(cv::Mat& image);

				virtual void release();


			protected:
				void run();
			private:
				std::string fileName(int current);
			private:
				TBS::Nullable <std::string> stream;

				bool finished;
				std::string uniqueName;

				int buffer;
				int current;

				Poco::Mutex m;
				Poco::Thread camThread;


		};

	} /* namespace BB */
} /* namespace TBS */

#endif /* FFMPEGGRABBER_H_ */
