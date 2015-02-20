/**
 * THIS FILE IS GENERATED BY genTool, DO NOT CHANGE IT!!!!!
 */
 #ifndef _NO_DBUS 
#ifndef _DATA_SERVICE_DBUS_H_
#define _DATA_SERVICE_DBUS_H_

#include <Poco/SharedPtr.h>
#include <Poco/BasicEvent.h>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <TBS/Services/Services.h>
#include <TBS/Services/DBus/DBusServices.h>
///includes
#include "BB/Services//Data.h"

namespace TBS {
	namespace Services {
		class CommunicationChannelHolder;
	}
}

namespace TBS { 
	namespace BB { 
		namespace Services { 
			namespace Data { 
				namespace DBus { 
       class GEN_SERVICE_API Client {
			public:
				typedef Poco::SharedPtr <Client> Ptr;
				
				//with given dbus dispatcher via DBusCommChannelProvider
				Client(TBS::Services::ICommChannelHolder::Ptr ch);
				//with default dbus dispatcher
				Client();
				~Client();
				
				TBS::BB::Services::Data::IDataCollector & DataCollector();
TBS::BB::Services::Data::IDataDistributor & DataDistributor();

				
		private: 
				TBS::Services::ICommChannelHolder::Ptr ch;
				std::unique_ptr<TBS::Services::CommunicationChannelHolder> comChannelHolder;
				TBS::BB::Services::Data::IDataCollector::Ptr datacollector_;
TBS::BB::Services::Data::IDataDistributor::Ptr datadistributor_;

			};
			
			
		class GEN_SERVICE_API Server {
			public:
				typedef Poco::SharedPtr<Server> Ptr;
				//with given dbus dispatcher via DBusCommChannelProvider
				Server(TBS::Services::ICommChannelHolder::Ptr ch);
				//uses default dbus dispatcher
				Server();
				~Server();
				
			public:
				TBS::Services::IServer::Ptr createDataCollector(TBS::BB::Services::Data::IDataCollector::Ptr impl);
TBS::Services::IServer::Ptr createDataDistributor(TBS::BB::Services::Data::IDataDistributor::Ptr impl);

				
			private:
				TBS::Services::ICommChannelHolder::Ptr ch;
		};
 } 
 } 
 } 
 } 
 } 

#endif //_DATA_SERVICE_H_
#endif
