/*
 * DbusServices.h
 *
 *  Created on: Oct 7, 2013
 *      Author: root
 */

#ifndef _NO_DBUS

#ifndef DBUSSERVICES_H_
#define DBUSSERVICES_H_
#include <memory>
#include "TBS/Services/Services.h"
#include <Poco/Mutex.h>
namespace TBS {
	namespace Services {

		class DBusCommChannelProvider {
			public:
				static IStoppableCommChannelHolder::Ptr createNewCommChannel(std::string name);

				static ICommChannelHolder::Ptr getDefaultCommChannel();
				static void terminateDefaultCommChannel();
			private:
				static IStoppableCommChannelHolder::Ptr getDefaultCommChannelImpl();
		};

	} /* namespace Services */
} /* namespace TBS */
#endif /* DBUSSERVICES_H_ */

#endif
