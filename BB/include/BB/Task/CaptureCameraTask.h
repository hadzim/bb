/*
 * CaptureCameraTask.h
 *
 *  Created on: Nov 1, 2014
 *      Author: dron
 */

#ifndef CaptureCameraTask_H_
#define CaptureCameraTask_H_
#include <BB/RuntimeStatus.h>
#include <BB/Task/Task.h>

namespace BB {

	class CaptureCameraTask {
		public:
			CaptureCameraTask(const Task & task);
			virtual ~CaptureCameraTask();

			int getDelay();
			int getSamples();

			static Task create(int delayInS = 5, int samples = 1);
		private:
			Task task;
	};

} /* namespace BB */

#endif /* CaptureCameraTask_H_ */
