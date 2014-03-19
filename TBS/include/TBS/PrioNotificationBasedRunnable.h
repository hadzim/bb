/*
 * PrioNotificationBasedRunnable.h
 *
 *  Created on: Sep 17, 2012
 *      Author: root
 */

#ifndef PRIONOTIFICATIONBASEDRUNNABLE_H_
#define PRIONOTIFICATIONBASEDRUNNABLE_H_

#include <Poco/PriorityNotificationQueue.h>
#include <Poco/Thread.h>
#include "TBS/TBS.h"
namespace TBS {


		/// TODO needs to be tested
		class TBS_API PrioNotificationBasedRunnable : public Poco::Runnable {
			public:
				PrioNotificationBasedRunnable(std::string threadName);
				virtual ~PrioNotificationBasedRunnable();

				bool isBgThread();
			protected:
				void enqueueNotification(Poco::Notification::Ptr notification, int priority);
				void run();
				virtual void processNotification(Poco::Notification::Ptr notification) = 0;
				void stop();
			protected:
				int queueSize();
			private:
				Poco::PriorityNotificationQueue queue;
				Poco::Mutex m;
				Poco::Thread thread;
				volatile bool stopped;
		};



}

#endif /* PRIONOTIFICATIONBASEDRUNNABLE_H_ */
