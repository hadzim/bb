/*
 * DBusCommChannelHolder.h
 *
 *  Created on: Oct 9, 2013
 *      Author: root
 */

#ifndef _NO_DBUS

#ifndef DBUSCOMMCHANNELHOLDER_H_
#define DBUSCOMMCHANNELHOLDER_H_
#include "TBS/Services/Services.h"
#include "Poco/Runnable.h"
#include <dbus-c++/eventloop-integration.h>
#include "Poco/Thread.h"

namespace TBS {
	namespace Services {



		class DBusCommChannelHolder : public IStoppableCommChannelHolder, public Poco::Runnable {
			public:
				DBusCommChannelHolder(std::string name);
				virtual ~DBusCommChannelHolder();

				virtual void start();
				virtual void stop();
				virtual bool isRunning();

				::DBus::BusDispatcher & dispatcher();
			protected:
				virtual void run();
			private:
				Poco::Thread t;
				::DBus::BusDispatcher dispatcher_;
				bool running;
		};

	} /* namespace Services */
} /* namespace TBS */
#endif /* DBUSCOMMCHANNELHOLDER_H_ */

#endif
