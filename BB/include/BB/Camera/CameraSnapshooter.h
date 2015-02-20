/*
 * CameraSnapshooter.h
 *
 *  Created on: Nov 7, 2014
 *      Author: dron
 */

#ifndef CAMERASNAPSHOOTER_H_
#define CAMERASNAPSHOOTER_H_
#include <Poco/SharedPtr.h>
#include <string>

namespace BB {

	class ICameraSnapshot {
		public:
			typedef Poco::SharedPtr<ICameraSnapshot> Ptr;
			virtual ~ICameraSnapshot() {

			}

			virtual void makeSnapshot(std::string snapshotDestination, int maxWidth) = 0;
	};

	class VideoSnapshot: public ICameraSnapshot {
		public:
			VideoSnapshot(std::string cameraUrl);
			virtual void makeSnapshot(std::string snapshotDestination, int maxWidth);
		private:
			std::string cameraUrl;

	};

	class FFMpegSnapshot: public ICameraSnapshot {
		public:
			FFMpegSnapshot(std::string cameraUrl);
			virtual void makeSnapshot(std::string snapshotDestination, int maxWidth);
		private:
			std::string cameraUrl;

	};

	class StaticSnapshot: public ICameraSnapshot {
		public:
			StaticSnapshot(std::string snapshotUrl, std::string username, std::string passwd);
			virtual void makeSnapshot(std::string snapshotDestination, int maxWidth);
		private:
			std::string snapshotUrl;
			std::string username;
			std::string passwd;

	};

} /* namespace BB */

#endif /* CAMERASNAPSHOOTER_H_ */
