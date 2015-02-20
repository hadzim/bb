/*
 * UpdateStatusTask.h
 *
 *  Created on: Nov 1, 2014
 *      Author: dron
 */

#ifndef UpdateStatusTask_H_
#define UpdateStatusTask_H_
#include <BB/RuntimeStatus.h>
#include <BB/Task/Task.h>

namespace BB {

	class UpdateStatusTask {
		public:
			UpdateStatusTask(const Task & task);
			virtual ~UpdateStatusTask();

			const RuntimeStatus & getStatus();
		private:
			RuntimeStatus status;
	};

} /* namespace BB */

#endif /* UpdateStatusTask_H_ */
