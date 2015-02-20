/*
 * Dump.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#include "Dump.h"
#include <iostream>
#include <Poco/Delegate.h>
#include "Poco/Thread.h"

#include "BB/Services/Data.h"
#include "BB/Services/DataSvc_DBus.h"
#include "BB/Sensor/SensorDataHelpers.h"
#include "BB/ServiceNotification.h"
#include "BB/Configuration.h"
#include <iostream>


namespace BB {


Dump::Dump() {

}

Dump::~Dump() {

}

int Dump::main(const std::vector<std::string>& args) {

	std::cout << "main Dump.start" << std::endl;

	{
		TBS::BB::Services::Data::DBus::Client::Ptr observer = new TBS::BB::Services::Data::DBus::Client();
		observer->DataDistributor().SensorDataReceived += Poco::delegate(this, &Dump::onData);
		this->waitForTerminationRequest();
		observer->DataDistributor().SensorDataReceived -= Poco::delegate(this, &Dump::onData);
	}

	std::cout << "main Dump.stop" << std::endl;


	return EXIT_OK;
}

void Dump::onData(TBS::BB::Services::Data::IDataDistributor::SensorDataReceivedArg & arg){

		BB::SensorData sd = BB::SensorDataHelpers::eventArg2SensorData(arg);
		std::cout << sd << std::endl;
	}

} /* namespace BB */
