/*
 * CollectorService.cpp
 *
 *  Created on: 18.3.2014
 *      Author: JV
 */

#include <BB/Service/DataSender.h>
#include "BB/Camera/CameraService.h"
#include <iostream>
#include <Poco/Delegate.h>

#include "BB/Task/CaptureCameraTask.h"
#include "BB/Forwarder/RemoteClient/RemoteClient.h"

#include <Poco/Delegate.h>
#include <sstream>

namespace BB {

	CameraWorker::CameraWorker(std::string cameraName, ICameraSnapshot::Ptr cs) : name(cameraName), cs(cs){
		LDEBUG("Cam") << "camera worker formwarder: " <<  cameraName << LE;
		timer.Timer += Poco::delegate(this, &CameraWorker::onTimer);



		SensorData cameraData(
				SensorData::Camera,
				cameraName,
				cameraName,
				"",
				SensorData::localNow(),
				SensorData::Sensor_Ok,
				0.0,
				"cam"
		);

		DataSender s;
		s.send(cameraData);

	}

	CameraWorker::~CameraWorker(){
		timer.Timer -= Poco::delegate(this, &CameraWorker::onTimer);
	}

	void CameraWorker::forward(const SensorData & d) {

	}

	void CameraWorker::forward(const RuntimeStatus & s) {

	}

	void CameraWorker::forward(const Notification & s) {

	}

	void CameraWorker::forward(const Task & t) {
		try {
			LDEBUG("Cam") << "task: " << t << LE;
			if (t.getDestination() == name || t.getDestination().empty() || t.getDestination() == "all" || t.getDestination() == "BB"){

				LDEBUG("Cam") << "task successfully received " << LE;

				BB::CaptureCameraTask cc(t);

				{
					Poco::Mutex::ScopedLock l(m);
					tasks.clear();
					CapturingTaskInfo inf;
					for (int i = 0; i < cc.getSamples(); i++){
						std::stringstream s;
						s << "Camera: " << this->name <<", from: " + t.getSource() << " (" << i + 1<< "/" << cc.getSamples() << ")";
						inf.info = s.str();
						tasks.push_back(inf);
					}
				}

				timer.start(0, cc.getDelay() * 1000);
				LDEBUG("Cam") << "task successfully processed " << LE;
			}

		} catch (Poco::Exception & e){
			LWARNING("Cam") << e.displayText() << LE;
		}
	}

	void CameraWorker::onTimer(TBS::SimpleTimer::TimerArg & t){
		LDEBUG("Cam") << "task successfully received " << LE;

		CapturingTaskInfo cc;
		{
			Poco::Mutex::ScopedLock l(m);
			if (tasks.empty()){
				this->timer.stop();
				return;
			}
			cc = tasks.front();
			tasks.pop_front();
		}


		LDEBUG("Cam") << "cam timer" << LE;

		std::string path = "/tmp/" + this->name + ".jpg";

		cs->makeSnapshot(path, 640);

		LDEBUG("Cam") << "cam snapshot received" << LE;

		BB::RemoteClient client;
		client.sendImage(path, cc.info);

		LDEBUG("Cam") << "cam processed " << LE;
	}

} /* namespace BB */
