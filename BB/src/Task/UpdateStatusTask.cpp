/*
 * UpdateStatusTask.cpp
 *
 *  Created on: Nov 1, 2014
 *      Author: dron
 */

#include <BB/Task/UpdateStatusTask.h>

#include "Poco/Exception.h"

namespace BB {

	UpdateStatusTask::UpdateStatusTask(const Task & task) {

		if (task.getWhat() != Task::UpdateStatus){
			throw Poco::Exception("Wrong task type");
		}
		status = RuntimeStatus::fromShortString(task.getStringParam("status"));

	}

	UpdateStatusTask::~UpdateStatusTask() {

	}

	const RuntimeStatus & UpdateStatusTask::getStatus(){
		return this->status;
	}


} /* namespace BB */
