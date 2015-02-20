/*
 * NotificationBasedRunnable.h
 *
 *  Created on: May 31, 2012
 *      Author: root
 */

#ifndef NOTIFICATIONBASEDRUNNABLE_H_
#define NOTIFICATIONBASEDRUNNABLE_H_
#include <Poco/NotificationQueue.h>
#include <Poco/Thread.h>
#include "Poco/Runnable.h"
#include <string>
#include "TBS/TBS.h"

namespace TBS {
/*
	class TBS_API NotificationQueue

	{
		public:
			NotificationQueue();

			~NotificationQueue();

			void enqueueNotification(Poco::Notification::Ptr pNotification);

			void enqueueUrgentNotification(Poco::Notification::Ptr pNotification);

			Poco::Notification* dequeueNotification();

			Poco::Notification* waitDequeueNotification();

			Poco::Notification* waitDequeueNotification(long milliseconds);

			void wakeUpAll();

			bool empty() const;

			int size() const;

			void clear();

			bool hasIdleThreads() const;

		protected:
			Poco::Notification::Ptr dequeueOne();

		private:
			typedef std::deque<Poco::Notification::Ptr> NfQueue;
			struct WaitInfo {
					Poco::Notification::Ptr pNf;
					Poco::Event nfAvailable;
			};
			typedef std::deque<WaitInfo*> WaitQueue;

			NfQueue _nfQueue;
			WaitQueue _waitQueue;
			mutable Poco::FastMutex _mutex;
	};
*/
	class TBS_API NotificationBasedRunnable: public Poco::Runnable {
		public:
			NotificationBasedRunnable(std::string threadName);
			virtual ~NotificationBasedRunnable();

			virtual bool isBgThread();
		protected:
			void enqueueNotification(Poco::Notification::Ptr notification);
			void run();
			virtual void processNotification(Poco::Notification::Ptr notification) = 0;
			void stop();
		protected:
			int queueSize();
		private:
			Poco::NotificationQueue queue;
			Poco::Thread thread;
			Poco::Mutex m;
			volatile bool stopped;
	};

} /* namespace TBS */
#endif /* NOTIFICATIONBASEDRUNNABLE_H_ */
