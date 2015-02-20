/*
 * Task.cpp
 *
 *  Created on: Feb 2, 2012
 *      Author: root
 */

#include "TBS/Task/Task.h"
#include <iostream>

#include "TBS/ThreadStatus.h"

namespace TBS {

	namespace Task {

		Task::TaskFinishedStatus::TaskFinishedStatus(std::string errorMessage) :
				statusType(STATUS_ERROR), errorMessage(errorMessage) {
		}
		Task::TaskFinishedStatus::TaskFinishedStatus(StatusType t) :
				statusType(t), errorMessage("") {
		}

		bool Task::TaskFinishedStatus::isOk() const {
			return statusType == STATUS_DONE;
		}
		bool Task::TaskFinishedStatus::isError() const {
			return statusType == STATUS_ERROR;
		}
		bool Task::TaskFinishedStatus::isCanceled() const {
			return statusType == STATUS_CANCELED;
		}
		std::string Task::TaskFinishedStatus::getErrorMessage() const {
			return this->errorMessage;
		}

		Task::Task(std::string name, INotificationWorker::Ptr nw_) :
				name(name), nw_(nw_) {

		}

		Task::~Task() {
			poco_assert(!this->isActive());
			std::cout << this->isActive() << std::endl;
			LTRACE("Task")<< "Task destructed: " << this->name << LE;

			if (!this->nw().isBgThread()) {
				if (!check.isNull()) {
					LTRACE("Task")<< "Task destructed - wait for output event: " << this->name << LE;
						this->check->wait();
					}
					LTRACE("Task") << "Task destructed - wait for output event: " << this->name << " done" << LE;
				}
			LTRACE("Task")<< "Task destructed: " << this->name << " done" << LE;
		}

		void Task::start() {
			if (!this->isActive()) {
				this->activate();
			}
		}

		void Task::cancel() {
			bool val = this->isActive();
			//this->BeforeCancel.notify(this, val);
			if (val) {
				this->passivate();

				TaskFinishedStatus status(TaskFinishedStatus::STATUS_CANCELED);
				CUSTOM_DUMP("Task_" + name, "Finished: canceled");
				if (!this->nw().isBgThread()) {
					this->notify<TaskFinishedStatus>(Finished, status);
				} else {
					this->Finished(this, status);
				}
			}

			//this->Finished.notify(this, status);
		}

		std::string Task::getName() {
			return this->name;
		}
		void Task::finished() {
			this->passivate();
			TaskFinishedStatus status(TaskFinishedStatus::STATUS_DONE);
			CUSTOM_DUMP("Task_" + name, "Finished: successful");
			this->notify<TaskFinishedStatus>(Finished, status);

			//this->Finished.notify(this, status);
		}
		void Task::failed(std::string message) {
			this->passivate();
			TaskFinishedStatus status(message);
			//status.errorMessage = message;
			CUSTOM_DUMP("Task_" + name, "Finished: failed with " + message);
			if (!this->nw().isBgThread()) {
				this->notify<TaskFinishedStatus>(Finished, status);
			} else {
				this->Finished(this, status);
			}
			//this->notify<TaskFinishedStatus>(Finished, status);

			//this->Finished.notify(this, status);
		}

		INotificationWorker & Task::nw() {
			return *(this->nw_.get());
		}

		bool Task::isActive() {
			return ActiveEventObject::isActive();
		}

	} /* namespace Task */

} /* namespace TBS */

std::ostream & operator<<(std::ostream & stream, const TBS::Task::Task::TaskFinishedStatus & s) {
	if (s.isOk()) {
		stream << "status: OK";
	}
	if (s.isCanceled()) {
		stream << "status: Canceled";
	}
	if (s.isError()) {
		stream << "status: Error (" << s.getErrorMessage() << ")";
	}
	return stream;
}
