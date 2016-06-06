/*
 * FFMpegGrabber.cpp
 *
 *  Created on: May 23, 2016
 *      Author: dron
 */

#include <Camera/FFMpegGrabber.h>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include "Poco/File.h"

namespace BB {
	namespace Camera {

		FFMpegGrabber::FFMpegGrabber(std::string uniqueName) : uniqueName(uniqueName) {
			finished = false;
			buffer = 5;
			current = 0;

			camThread.start(*this);
		}

		FFMpegGrabber::~FFMpegGrabber() {
			finished = true;
			camThread.join();
		}

		void FFMpegGrabber::run(){
			std::string fnameNew = fileName(0);
			std::string fnameOld = fileName(1);
			while (!finished){
				if (stream.isSet()){

					std::stringstream cmd;
					cmd << "ffmpeg  -i " << stream.val()<< " -f image2  -vframes 1 -ss 00:00:02.000 " << fnameNew;
					std::cout << cmd.str() << std::endl;
					system(cmd.str().c_str());
					{
						Poco::Mutex::ScopedLock l(m);
						Poco::File f(fnameNew);
						if (f.exists()){
							f.moveTo(fnameOld);
						}
					}
					Poco::Thread::sleep(1000);

				} else {
					Poco::Thread::sleep(2000);
				}
			}
		}

		std::string FFMpegGrabber::fileName(int current){
			std::stringstream s;
			s << "/tmp/camera-" << uniqueName << "-" << current << ".png";
			return s.str();
		}

		bool FFMpegGrabber::open(const std::string& filename){
			stream.set(filename);
			return true;
		}

		bool FFMpegGrabber::open(int device){
			return false;
		}

		bool FFMpegGrabber::isOpened() const {
			return stream.isSet();
		}

		IGrabber& FFMpegGrabber::operator >> (cv::Mat& image){
			std::string fnameOld = fileName(1);
			{
				Poco::Mutex::ScopedLock l(m);
				Poco::File f(fnameOld);
				if (f.exists()){
					image = cv::imread(fnameOld);
					//f.remove(true);
				}
			}
			return *this;
		}

		bool FFMpegGrabber::read(cv::Mat& image){
			std::string fnameOld = fileName(1);
			{
				Poco::Mutex::ScopedLock l(m);
				Poco::File f(fnameOld);
				if (f.exists()){
					image = cv::imread(fnameOld);
					//f.remove(true);
					return true;
				}
			}
			return false;
		}

		void FFMpegGrabber::release(){
			stream.reset();
		}

	} /* namespace BB */
} /* namespace TBS */
