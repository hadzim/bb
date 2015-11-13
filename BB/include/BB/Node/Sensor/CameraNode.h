/*
 * ContactNode.h
 *
 *  Created on: Feb 3, 2013
 *      Author: root
 */

#ifndef CameraNode_H_
#define CameraNode_H_
#include <BB/Camera/IGrabber.h>
#include <BB/Node/BasicNode.h>
#include <Poco/Runnable.h>

#include "TBS/Nullable.h"

namespace BB {

		class CameraNode: public BasicNode, private Poco::Runnable {
			public:
				CameraNode(Camera::IGrabber::Ptr grabber, std::string uid, int period);
				~CameraNode();
			protected:
				virtual AllData read();
			private:
				void run();
			private:
				Camera::IGrabber::Ptr grabber;

				Poco::Thread t;
				Poco::Mutex m;
				TBS::Nullable<cv::Mat> current;
				bool stopBg;

		};

} /* namespace BB */
#endif /* DALLASTEMPERATURESENSOR_H_ */
