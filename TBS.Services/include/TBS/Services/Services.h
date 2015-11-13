/*
 * Services.h
 *
 *  Created on: Oct 7, 2013
 *      Author: root
 */

#ifndef TBS_SERVICES_H_
#define TBS_SERVICES_H_
#include <Poco/SharedPtr.h>
#include <Poco/Exception.h>
#include <vector>
#include "TBS/Services/Types.h"

namespace TBS {
	namespace Services {

		typedef std::vector <unsigned char> Binary;

		class GEN_SERVICE_API ICommChannelHolder {
			public:
				typedef Poco::SharedPtr <ICommChannelHolder> Ptr;
				virtual ~ICommChannelHolder();
		};

		class IStoppableCommChannelHolder : public ICommChannelHolder {
			public:
				typedef Poco::SharedPtr <IStoppableCommChannelHolder> Ptr;
				virtual ~IStoppableCommChannelHolder();

				virtual void start() = 0;
				virtual void stop() = 0;
				virtual bool isRunning() = 0;
		};


		class GEN_SERVICE_API IServer {
			public:
				typedef Poco::SharedPtr<IServer> Ptr;

				virtual ~IServer();
		};

		#define APIMACRO

		POCO_DECLARE_EXCEPTION(APIMACRO, ServiceException, Poco::Exception);
		POCO_DECLARE_EXCEPTION(APIMACRO, RuntimeServiceException, ServiceException);
		POCO_DECLARE_EXCEPTION(APIMACRO, ChannelServiceException, ServiceException);

	} // namespace Services

} // namespace TBS



#endif /* SERVICES_H_ */
