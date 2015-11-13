/*
 * CameraSnapshooter.cpp
 *
 *  Created on: Nov 7, 2014
 *      Author: dron
 */

#include "BB/Camera/CameraSnapshooter.h"

#include <opencv2/opencv.hpp>

#include "Poco/Exception.h"

#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/FilePartSource.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPResponse.h>
#include <string>

#include <Poco/StreamCopier.h>

#include "TBS/Log.h"

#include <fstream>
#include <sstream>
#include <Poco/URI.h>
#include <Poco/Net/HTTPCredentials.h>

namespace BB {

	VideoSnapshot::VideoSnapshot(std::string cameraUrl) :
			cameraUrl(cameraUrl) {

	}

	void VideoSnapshot::makeSnapshot(std::string snapshotDestination, int maxWidth) {
		std::cout << "1" << std::endl;

		cv::VideoCapture video(cameraUrl);

		if (!video.isOpened()) {
			std::cout << "Cannot open the video file" << std::endl;
			throw Poco::Exception("cannot open stream");
		}

		cv::Mat m;

		std::cout << "2" << std::endl;

		//video >> m;

		bool val = video.grab();
		if (!val) {
			std::cout << "Cannot grab" << std::endl;
		}
		std::cout << "2a" << std::endl;

		video.retrieve(m);

		std::cout << "3" << std::endl;

		if (m.cols > maxWidth) {
			cv::resize(m, m, cv::Size(maxWidth, m.rows * maxWidth / m.cols));
		}

		cv::imwrite(snapshotDestination, m);

		std::cout << "4" << std::endl;
	}


	FFMpegSnapshot::FFMpegSnapshot(std::string cameraUrl) :
			cameraUrl(cameraUrl) {
	}

	void FFMpegSnapshot::makeSnapshot(std::string snapshotDestination, int maxW) {
		std::stringstream s;
		s << "ffmpeg -i " << cameraUrl << " -ss 00:00:00.500 -f image2 -vframes 1 " << snapshotDestination;
		int ret = system(s.str().c_str());
		LWARNING("Cam") << s.str() << " rc: " << ret << LE;
	}



	StaticSnapshot::StaticSnapshot(std::string snapshotUrl, std::string username, std::string passwd) :
			snapshotUrl(snapshotUrl), username(username), passwd(passwd) {

	}

	void StaticSnapshot::makeSnapshot(std::string snapshotDestination, int maxWidth) {
		try {
			Poco::URI uri(snapshotUrl);
			std::string path(uri.getPathAndQuery());
			if (path.empty()) {
				path = "/";
			}

			Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());
			Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, path, Poco::Net::HTTPMessage::HTTP_1_1);
			Poco::Net::HTTPResponse response;

			if (username.length()) {
				Poco::Net::HTTPCredentials credentials(username, passwd);
				credentials.authenticate(request, response);
			}

			session.sendRequest(request);
			std::istream& rs = session.receiveResponse(response);
			std::cout << response.getStatus() << " " << response.getReason() << std::endl;

			{
				std::ofstream outimage(snapshotDestination, std::ios::binary);
				Poco::StreamCopier::copyStream(rs, outimage);
			}

		} catch (Poco::Exception& exc) {
			std::cerr << exc.displayText() << std::endl;
			throw;
		}
	}

} /* namespace BB */
