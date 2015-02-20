/*
 * BgSender.h
 *
 *  Created on: Feb 2, 2013
 *      Author: root
 */

#ifndef BGSENDER_H_
#define BGSENDER_H_
#include <BB/Notification.h>
#include "IForwarder.h"
#include <Poco/Notification.h>
#include "TBS/NotificationBasedRunnable.h"

namespace BB {

	class MesssageNotification: public Poco::Notification {
		public:
			typedef Poco::AutoPtr<MesssageNotification> Ptr;
			MesssageNotification(const SensorData & d);
			const SensorData & message() const;
		private:
			SensorData m;
	};

	class StatusNotification: public Poco::Notification {
		public:
			typedef Poco::AutoPtr<StatusNotification> Ptr;
			StatusNotification(const RuntimeStatus & s);
			const RuntimeStatus & status() const;
		private:
			RuntimeStatus s;
	};

	class TaskNotification: public Poco::Notification {
		public:
			typedef Poco::AutoPtr<TaskNotification> Ptr;
			TaskNotification(const Task & t);
			const Task & task() const;
		private:
			Task t;
	};

	class NtfNotification: public Poco::Notification {
		public:
			typedef Poco::AutoPtr<NtfNotification> Ptr;
			NtfNotification(const BB::Notification & n);
			const BB::Notification & ntf() const;
		private:
			BB::Notification n;
	};


	class BgForwarder: public IForwarder, public TBS::NotificationBasedRunnable {
		public:
			BgForwarder(std::string name, IForwarder::Ptr wrappedForwarder);
			virtual ~BgForwarder();

			virtual void forward(const SensorData & m);
			virtual void forward(const RuntimeStatus & m);
			virtual void forward(const Task & t);
			virtual void forward(const Notification & n);

		private:
			void processNotification(Poco::Notification::Ptr notification);
		private:
			IForwarder::Ptr wrappedForwarder;
	};

} /* namespace TBS */
#endif /* BGSENDER_H_ */
