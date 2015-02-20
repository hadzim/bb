/*
 * CaptureCameraTask.cpp
 *
 *  Created on: Nov 1, 2014
 *      Author: dron
 */

#include <BB/Task/CaptureCameraTask.h>

#include "Poco/Exception.h"

#include "json/writer.h"

namespace BB {

	Task CaptureCameraTask::create(int delay, int samples){
		Json::Value v;
		v["delay"] = Json::Value(delay);
		v["samples"] = Json::Value(samples);
		Json::FastWriter w;
		Task t(Task::CaptureCamera, w.write(v));
		return t;
	}

	CaptureCameraTask::CaptureCameraTask(const Task & task) : task(task) {

		if (task.getWhat() != Task::CaptureCamera) {
			throw Poco::Exception("Wrong task type");
		}
		//status = RuntimeStatus::fromShortString(task.getStringParam("status"));

	}

	CaptureCameraTask::~CaptureCameraTask() {

	}

	int CaptureCameraTask::getDelay() {
		return this->task.getIntParam("delay");
	}
	int CaptureCameraTask::getSamples() {
		return this->task.getIntParam("samples");
	}

/*
 RuntimeStatus CaptureCameraTask::getStatus(){
 return this->status;
 }
 */

} /* namespace BB */
