/*
 * SynchronousTask.cpp
 *
 *  Created on: Feb 4, 2012
 *      Author: root
 */

#include "TBS/Task/Utils/SynchronousTaskExecutor.h"
#include "TBS/Log.h"

namespace TBS {
	namespace Task {

		SynchronousTaskExecutor::SynchronousTaskExecutor(Task::Ptr task) :
				task(task) {
		}
		Task::TaskFinishedStatus SynchronousTaskExecutor::runInternally(bool timeoutSet, int timeout) {
			try {
				LTRACE("Task")<< "sync exec: runInternally: " << task->getName() << LE;
					this->event.reset();
					LTRACE("Task") << "sync exec: delegate add to Finished timeout: " << timeout << LE;
					task->Finished += Poco::delegate(this, &SynchronousTaskExecutor::onFinished);
					task->start();
					if (timeoutSet) {
						try {
							this->event.wait(timeout);
						} catch (Poco::Exception & e) {
							task->cancel();
							task->Finished -= Poco::delegate(this, &SynchronousTaskExecutor::onFinished);
							LWARNING("Task") << "SynchronousTaskExecutor '" << task->getName() << "' exception: " << e.what() << LE;
							throw;
						}
					} else {
						this->event.wait();
					}

					task->Finished -= Poco::delegate(this, &SynchronousTaskExecutor::onFinished);
					LTRACE("Task") << "delegate SynchronousTaskExecutor::onFinished removed form Finished" << LE;
					if (status.isError()) {
						throw Poco::Exception(status.getErrorMessage());
					}
					LTRACE("Task") << "SynchronousTaskExecutor::runInternally finished - " << task->getName() << LE;
					return status;
				} catch (...) {
					task->cancel();
					task->Finished -= Poco::delegate(this, &SynchronousTaskExecutor::onFinished);
					throw;
				}
			}

		Task::TaskFinishedStatus SynchronousTaskExecutor::run() {
			return this->runInternally(false);
		}
		Task::TaskFinishedStatus SynchronousTaskExecutor::run(int timeout) {
			return this->runInternally(true, timeout);
		}

		void SynchronousTaskExecutor::onFinished(Task::TaskFinishedStatus & status) {
			this->status = status;
			this->event.set();
		}

	} /* namespace Tasks */

} /* namespace TBS */
