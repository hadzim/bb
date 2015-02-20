/*
 * CollectorService.cpp
 *
 *  Created on: 18.3.2014
 *      Author: JV
 */

#include "CollectorService.h"
#include <iostream>
#include <Poco/Delegate.h>
#include <Poco/Util/AbstractConfiguration.h>
#include <Poco/Util/XMLConfiguration.h>
#include "BB/Services/DataSvc_DBus.h"


#include "BB/ServiceNotification.h"

namespace BB {

	CollectorService::CollectorService() {
	}

	int CollectorService::main(const std::vector<std::string>& args) {
		try {


			TBS::initLogs("Collector", 8, "/tmp/");

			std::cout << "Collector starts" << std::endl;

			TBS::BB::Services::Data::DBus::Server::Ptr dbusServer = new TBS::BB::Services::Data::DBus::Server();
			{

				DataDistributor::Ptr distributor = new DataDistributor();
				DataCollector::Ptr collector = new DataCollector(distributor);
				TBS::Services::IServer::Ptr sc = dbusServer->createDataCollector(collector);
				TBS::Services::IServer::Ptr sd = dbusServer->createDataDistributor(distributor);

				BB::ServiceNotification::serviceReady();

				this->waitForTerminationRequest();

				BB::ServiceNotification::serviceDisabled();
				//Poco::Thread::sleep(4000);
			}

			std::cout << "Collector finished" << std::endl;
		} catch (std::exception & e) {
			std::cout << "exc: " << e.what() << std::endl;
		}
		return EXIT_OK;
	}

} /* namespace BB */
