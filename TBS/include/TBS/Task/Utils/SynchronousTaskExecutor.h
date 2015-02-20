/*
 * SynchronousTask.h
 *
 *  Created on: Feb 4, 2012
 *      Author: root
 */

#ifndef TBSSYNCHRONOUSTASK_H_
#define TBSSYNCHRONOUSTASK_H_
#include "../Task.h"
#include <Poco/Delegate.h>

namespace TBS {
			namespace Task {

				class SynchronousTaskExecutor {
					public:
						SynchronousTaskExecutor(Task::Ptr task);
						Task::TaskFinishedStatus run();
						Task::TaskFinishedStatus run(int timeout);

					private:
						void onFinished(Task::TaskFinishedStatus & status);

						Task::TaskFinishedStatus runInternally(bool timeoutSet, int timeout = 0);

					private:
						Task::Ptr task;
						Task::TaskFinishedStatus status;
						Poco::Event event;

				};

			} /* namespace Tasks */


} /* namespace TBS */
#endif /* SYNCHRONOUSTASK_H_ */
