/*
 * Task.h
 *
 *  Created on: Feb 2, 2012
 *      Author: root
 */

#ifndef TBSNewDEVICE_TASK_H_
#define TBSNewDEVICE_TASK_H_
#include "TBS/ActiveEventObject.h"
#include <Poco/SharedPtr.h>
#include <TBS/OutputNotification.h>
#include "TBS/INotificationWorker.h"
#include "TBS/Log.h"
#include <memory>
#include <iostream>

#include "TBS/CommandNotification.h"

namespace TBS {

	namespace Task {
		class Task: protected ActiveEventObject {
			public:
				class TaskFinishedStatus {
					public:
						enum StatusType {
							STATUS_DONE, STATUS_ERROR, STATUS_CANCELED,
						};

						TaskFinishedStatus(std::string errorMessage);
						TaskFinishedStatus(StatusType t = STATUS_DONE);

						bool isOk() const;
						bool isError() const;
						bool isCanceled() const;
						std::string getErrorMessage() const;
					private:
						StatusType statusType;
						std::string errorMessage;
				};

				typedef Poco::SharedPtr<Task> Ptr;

				Task(std::string name, INotificationWorker::Ptr nw_);
				virtual ~Task();

				Task(const Task & t);

				void start();
				void cancel();
				Poco::BasicEvent<TaskFinishedStatus> Finished;

				bool isActive();

				std::string getName();
			protected:

				void finished();
				void failed(std::string message);

				template<class Argument>
				void notify(Poco::BasicEvent<Argument> & event, Argument & value) {
					LTRACE("Task") << "Task " << this->getName() << " notify " << LE;

					typename OutputEventCommandCheckable<Argument>::Ptr o = new OutputEventCommandCheckable<Argument>(this->getName(), this, event, value);
					this->check = o->getCheck();
					CommandExecutorEnqueueAndContinue::execute(this->nw(), o);
					LTRACE("Task") << "Task " << this->getName() << " notify enqued" << LE;
				}

			protected:
				INotificationWorker & nw();
			private:

				std::string name;
				INotificationWorker::Ptr nw_;

				Poco::SharedPtr<Poco::Event> check;
		};

		/**
		 * to keep task as a class member
		 * do not use together with atomic task provider
		 * */
		template<class TaskType>
		class TaskHolder {
			public:
				typedef std::auto_ptr<TaskType> APtr;
				//takes ownership
				TaskHolder(APtr task_) :
						task_(task_) {

				}
				//takes ownership
				TaskHolder(TaskType * task_) :
						task_(APtr(task_)) {
				}
				~TaskHolder() {
					task_->cancel();
				}
				TaskType & task() {
					return *(task_.get());
				}
				TaskType * operator->() {
					return (task_.get());
				}
			private:
				APtr task_;
		};
	}

} /* namespace TBS */

std::ostream & operator<<(std::ostream & stream, const TBS::Task::Task::TaskFinishedStatus & s);

#endif /* TASK_H_ */
