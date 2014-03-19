/*
 * ConnectionWrapper.h
 *
 *  Created on: Sep 29, 2013
 *      Author: root
 */

#ifndef _NO_DBUS

#ifndef CONNECTIONWRAPPER_H_
#define CONNECTIONWRAPPER_H_
#include <dbus-c++/connection.h>
#include <Poco/BasicEvent.h>

namespace TBS {
		namespace Services {

			class ConnectionWrapper {
				public:

				  static DBus::Connection SystemBus(DBus::Dispatcher & disp);

				  static DBus::Connection SessionBus(DBus::Dispatcher & disp);

				  static DBus::Connection ActivationBus(DBus::Dispatcher & disp);
				private:
				  static Poco::Mutex m;
			};

		} /* namespace Services */
} /* namespace TBS */
#endif /* CONNECTIONWRAPPER_H_ */

#endif
