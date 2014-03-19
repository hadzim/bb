#ifndef _NO_DBUS
/*
 * DbusServices.cpp
 *
 *  Created on: Oct 7, 2013
 *      Author: root
 */


#include "TBS/Services/DBus/DBusServices.h"

//#include "TBS/Log.h"
#include <dbus-c++/eventloop-integration.h>
#include "Poco/SharedPtr.h"
#include "TBS/Services/DBus/DBusCommChannelHolder.h"

namespace TBS {
	namespace Services {

		IStoppableCommChannelHolder::Ptr DBusCommChannelProvider::createNewCommChannel(std::string name) {
			return new DBusCommChannelHolder(name);
		}

		IStoppableCommChannelHolder::Ptr DBusCommChannelProvider::getDefaultCommChannelImpl(){
			static IStoppableCommChannelHolder::Ptr ch;
			if (ch.isNull()){
				ch = new DBusCommChannelHolder("global");
				ch->start();
			}
			return ch;
		}

		ICommChannelHolder::Ptr DBusCommChannelProvider::getDefaultCommChannel() {
			return getDefaultCommChannelImpl();
		}
		void DBusCommChannelProvider::terminateDefaultCommChannel(){
			getDefaultCommChannelImpl()->stop();
		}

	}
}

#endif
