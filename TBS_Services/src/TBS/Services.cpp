/*
 * Services.h
 *
 *  Created on: Oct 7, 2013
 *      Author: root
 */

#ifndef SERVICES_H_
#define SERVICES_H_
#include "TBS/Services/Services.h"
#include <typeinfo>
#include <iostream>

namespace TBS {
	namespace Services {
		ICommChannelHolder::~ICommChannelHolder() {
		}

		IStoppableCommChannelHolder::~IStoppableCommChannelHolder(){}

		IServer::~IServer() {
			std::cout << "IServer deleted" << std::endl;
		}

		POCO_IMPLEMENT_EXCEPTION(ServiceException, Poco::Exception, "ServiceException");
		POCO_IMPLEMENT_EXCEPTION(RuntimeServiceException, ServiceException, "ServiceRuntimeException");
		POCO_IMPLEMENT_EXCEPTION(ChannelServiceException, ServiceException, "ServiceChannelException");

	} // namespace Services

} // namespace TBS

#endif /* SERVICES_H_ */
