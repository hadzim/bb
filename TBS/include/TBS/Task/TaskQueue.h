/*
 * TaskQueue.h
 *
 *  Created on: Mar 26, 2014
 *      Author: dron
 */

#ifndef TBSTASKQUEUE_H_
#define TBSTASKQUEUE_H_

#include <list>
#include <Poco/SharedPtr.h>
#include "Task.h"

namespace TBS {
	namespace Task {

		template<class TaskType>
		class TaskQueue {
			public:

				typedef typename TaskType::Ptr TaskPtr;

				typedef Poco::SharedPtr<TaskQueue> Ptr;

				std::list<TaskPtr> pendingTasks;
				TaskPtr activeTask;
				Poco::Mutex m;
				INotificationWorker::Ptr nw;

				TaskQueue(INotificationWorker::Ptr nw) :
						nw(nw) {

				}
				~TaskQueue() {
					Poco::Mutex::ScopedLock l(m);
					for (typename std::list<TaskPtr>::iterator i = pendingTasks.begin(); i != pendingTasks.end(); i++) {
						(*i)->cancel();
					}
					pendingTasks.clear();
					if (this->activeTask.get()) {
						this->activeTask->cancel();
					}
					this->activeTask = NULL;
				}

				void addTask(TaskPtr t) {
					Poco::Mutex::ScopedLock l(m);
					this->pendingTasks.push_back(t);

					this->runNextPendingTask();
				}
				void cancelTask(TaskPtr t) {
					Poco::Mutex::ScopedLock l(m);
					pendingTasks.remove(t);
					if (this->activeTask == t) {
						this->activeTask->cancel();
					}
					this->runNextPendingTask();
				}
			private:
				void runNextPendingTask() {
					ICommand::Ptr cmd = new Command<TaskQueue>("runNextPendingTask", this, &TaskQueue::runNextPendingTaskImpl);
					CommandExecutorEnqueueAndContinue::execute(nw, cmd);
				}
				void runNextPendingTaskImpl() {
					Poco::Mutex::ScopedLock l(m);
					if (this->activeTask.isNull() && !pendingTasks.empty()) {
						activeTask = pendingTasks.front();
						pendingTasks.pop_front();
						activateActiveTask(true);
					}
				}

				void activateActiveTask(bool activate) {
					//should be locked  by caller
					if (activeTask) {
						if (activate) {
							activeTask->Finished += Poco::delegate(this, &TaskQueue::onActiveTaskFinished);
							activeTask->start();
						} else {
							activeTask->Finished -= Poco::delegate(this, &TaskQueue::onActiveTaskFinished);
							activeTask->cancel();
							activeTask = NULL;
							this->runNextPendingTask();
						}
					}
				}
				void onActiveTaskFinished(Task::TaskFinishedStatus & status) {
					Poco::Mutex::ScopedLock l(m);
					this->activateActiveTask(false);
				}

		};

	}
} /* namespace TBS */

#endif /* TASKQUEUE_H_ */
