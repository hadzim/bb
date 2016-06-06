/*
 * ContactNode.h
 *
 *  Created on: Feb 3, 2013
 *      Author: root
 */

#ifndef CameraNode_H_
#define CameraNode_H_
#include <BB/Camera/IGrabber.h>
#include <BB/Node/Switch/SwitchNode.h>
#include <Poco/Runnable.h>

#include "TBS/Nullable.h"

#include "TBS/SimpleTimer.h"

namespace BB {

		class CameraNode: public SwitchNode, private Poco::Runnable {
			public:
				CameraNode(std::string uid);
				~CameraNode();
			protected:
				void performSwitch(bool on);
			private:
				void run();
				void onTimer(TBS::SimpleTimer::TimerArg & a);

				void onChanged(SettingsValues  & s);
				void updateHost();
			private:
				Camera::IGrabber::Ptr grabber;

				Poco::Thread t;
				Poco::Mutex m;
				TBS::Nullable<cv::Mat> current;
				bool stopBg;

				TBS::SimpleTimer timer;

				Poco::Mutex mSettings;
				std::string streamHost;
				int frequency;
				bool cfgChanged;
				bool isffmpeg;
		};

} /* namespace BB */
#endif /* DALLASTEMPERATURESENSOR_H_ */
