/*
 * CameraService.h
 *
 *  Created on: 18.3.2014
 *      Author: JV
 */

#ifndef CAMCOLLECTORSERVICE_H_
#define CAMCOLLECTORSERVICE_H_



#include <iostream>
#include <Poco/SharedPtr.h>
#include "TBS/Log.h"

#include <Poco/Delegate.h>

#include "BB/Forwarder/IForwarder.h"

#include "CameraSnapshooter.h"
#include "TBS/SimpleTimer.h"
#include <deque>

namespace BB {

	class CameraWorker: public IForwarder {
		public:
			CameraWorker(std::string cameraName, ICameraSnapshot::Ptr cs);
			virtual ~CameraWorker();

			virtual void forward(const SensorData & d);
			virtual void forward(const RuntimeStatus & s);
			virtual void forward(const Task & t);
			virtual void forward(const Notification & n);

		private:
			void onTimer(TBS::SimpleTimer::TimerArg & t);

		private:


			TBS::SimpleTimer timer;

			std::string name;
			ICameraSnapshot::Ptr cs;

			struct CapturingTaskInfo {
				std::string info;
			};
			std::deque <CapturingTaskInfo> tasks;
			Poco::Mutex m;
	};
}

#endif /* COLLECTORSERVICE_H_ */
