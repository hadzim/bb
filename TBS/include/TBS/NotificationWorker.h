/*
 * NotificationWorker.h
 *
 *  Created on: 30.11.2011
 *      Author: Honza
 */

#ifndef TBS_TSERIES_NOTIFICATIONWORKER_H_
#define TBS_TSERIES_NOTIFICATIONWORKER_H_

#include <Poco/Notification.h>
#include <Poco/NotificationCenter.h>
#include <Poco/SharedPtr.h>
#include "TBS/NotificationBasedRunnable.h"
#include <Poco/NObserver.h>
#include "INotificationWorker.h"
#include <list>
#include "TBS.h"

namespace TBS {

	class TBS_API NotificationWorker: public INotificationWorker, private NotificationBasedRunnable {

		public:

			typedef Poco::SharedPtr<NotificationWorker> Ptr;

			NotificationWorker(std::string name);
			virtual ~NotificationWorker();

			virtual void addObserver(const Poco::AbstractObserver& observer);
			virtual void removeObserver(const Poco::AbstractObserver& observer);

			virtual void postNotification(Poco::Notification::Ptr pNotification);
			virtual bool isBgThread();
		protected:
			void processNotification(Poco::Notification::Ptr notification);

		private:

			Poco::NotificationCenter center;
			std::string name;
	};

	class TBS_API NotificationObserver {

		public:
		NotificationObserver();
		~NotificationObserver();

		template<class Holder, class Notif>
		void init(INotificationWorker::Ptr nw, Holder& object, void(Holder::*method)(const Poco::AutoPtr<Notif> &)) {
			this->nw = nw;
			Poco::NObserver<Holder, Notif> o(object, method);
			this->set(o.clone());
		}
		void reset();
		private:
		void set(Poco::AbstractObserver * o);

		private:
		Poco::AbstractObserver * observer;
		INotificationWorker::Ptr nw;
	};

	class TBS_API NotificationObservers {
			typedef std::list<Poco::AbstractObserver *> Observers;
		public:

			NotificationObservers(INotificationWorker::Ptr nw);
			~NotificationObservers();

			template<class Holder, class Notif>
			void add(Holder& object, void (Holder::*method)(const Poco::AutoPtr<Notif> &)) {
				Poco::NObserver<Holder, Notif> o(object, method);
				this->addInternally(o.clone());
			}
			void clear();
		private:
			void addInternally(Poco::AbstractObserver * o);
		private:
			Observers observers;
			INotificationWorker::Ptr nw;
	};

	TBS_API void addCommandObserver(INotificationWorker &nw);
	TBS_API void removeCommandObserver(INotificationWorker & nw);

}

#endif /* NOTIFICATIONWORKER_H_ */
