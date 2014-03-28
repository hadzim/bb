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
#include "BB/Services/SensorSvc_DBus.h"

#include "BB/ServiceNotification.h"

namespace BB {

CollectorService::CollectorService() {
}

int CollectorService::main(const std::vector<std::string>& args) {
	try {
		std::cout << "Collector starts" << std::endl;

		TBS::BB::Services::Sensor::DBus::Server::Ptr dbusServer =
				new TBS::BB::Services::Sensor::DBus::Server();
		{

			SensorDistributor::Ptr distributor = new SensorDistributor();
			SensorCollector::Ptr collector = new SensorCollector(distributor);
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
