/*
 * BgSender.h
 *
 *  Created on: Feb 2, 2013
 *      Author: root
 */

#ifndef BGSENDER_H_
#define BGSENDER_H_
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

	class BgForwarder: public IForwarder, public TBS::NotificationBasedRunnable {
		public:
			BgForwarder(std::string name, IForwarder::Ptr wrappedForwarder);
			virtual ~BgForwarder();

			virtual void forward(const SensorData & m);

		private:
			void processNotification(Poco::Notification::Ptr notification);
		private:
			IForwarder::Ptr wrappedForwarder;
	};

} /* namespace TBS */
#endif /* BGSENDER_H_ */
