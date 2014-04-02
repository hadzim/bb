/*
 * main.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */



#include "BB/App.h"
#include "BB/ForwardApp.h"
#include "BB/Forwarder/RemoteClient/RemoteClientForwarder.h"
#include "BB/Forwarder/FilteringForwarder.h"
#include "BB/Forwarder/BgForwarder.h"

#include "BB/Forwarder/Filter.h"


#include "BB/Configuration.h"

namespace BB {

	class EmptyFwd : public IForwarder {
			void forward(const SensorData & data){
				//std::cout << "FINAL: Received: " << data << std::endl;
			}
	};


	class Factory : public IForwarderFactory {
		virtual IForwarder::Ptr createForwarder(){

			std::string url 		= Configuration::initCfg("RemoteClient", "url", 		"127.0.0.1");
			int			port		= Configuration::initCfg("RemoteClient", "port", 		80);
			std::string query 		= Configuration::initCfg("RemoteClient", "query", 		"/mereni2/www/");
			std::string projectID 	= Configuration::initCfg("RemoteClient", "projectID", 	"1");


			//create remote client
			IForwarder::Ptr remoteClient = new RemoteClientForwarder(url,port,query,projectID);

			//forward on background
			BgForwarder::Ptr bg = new BgForwarder("rc-bg", remoteClient);

			//filter only temperature each x minutes - take care of timespan format !!!
			FilteringForwarder::Ptr filter = new FilteringForwarder(bg);
			filter->addFilter(
				new TimespanFilter(SensorData::Temperature, Poco::Timespan(0, 0, 5, 0, 0))
			);

			return filter;
		}
	};
}

FWD_BB_MAIN(BB::Factory)


