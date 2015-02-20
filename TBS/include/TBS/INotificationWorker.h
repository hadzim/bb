/*
 * INotificationWorker.h
 *
 *  Created on: Feb 1, 2014
 *      Author: dron
 */

#ifndef INOTIFICATIONWORKER_H_
#define INOTIFICATIONWORKER_H_
#include <Poco/Notification.h>
#include <Poco/SharedPtr.h>

#include "Poco/AbstractObserver.h"
#include "TBS.h"

namespace TBS {

		class TBS_API INotificationWorker {
			public:
				typedef Poco::SharedPtr <INotificationWorker> Ptr;
				virtual ~INotificationWorker();

				virtual void addObserver(const Poco::AbstractObserver& observer) = 0;
				virtual void removeObserver(const Poco::AbstractObserver& observer) = 0;

				virtual void postNotification(Poco::Notification::Ptr pNotification) = 0;

				virtual bool isBgThread() = 0;
		};

} /* namespace TBS */

#endif /* INOTIFICATIONWORKER_H_ */
