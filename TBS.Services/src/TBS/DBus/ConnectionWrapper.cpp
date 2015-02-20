/*
 * ConnectionWrapper.cpp
 *
 *  Created on: Sep 29, 2013
 *      Author: root
 */

#ifndef _NO_DBUS

#include "dbus-c++/connection.h"
#include "dbus-c++/eventloop.h"
#include "dbus-c++/eventloop-integration.h"
#include "TBS/Services/DBus/ConnectionWrapper.h"

namespace TBS {
		namespace Services {

			Poco::Mutex ConnectionWrapper::m;

			DBus::Connection ConnectionWrapper::SystemBus(DBus::Dispatcher & disp) {
				Poco::Mutex::ScopedLock l(m);
				DBus::default_dispatcher = &disp;
				return DBus::Connection::SystemBus();

			}

			DBus::Connection ConnectionWrapper::SessionBus(DBus::Dispatcher & disp) {
				Poco::Mutex::ScopedLock l(m);
				DBus::default_dispatcher = &disp;
				return DBus::Connection::SessionBus();
			}

			DBus::Connection ConnectionWrapper::ActivationBus(DBus::Dispatcher & disp) {
				Poco::Mutex::ScopedLock l(m);
				DBus::default_dispatcher = &disp;
				return DBus::Connection::ActivationBus();
			}


		} /* namespace Services */
} /* namespace TBS */

#endif
