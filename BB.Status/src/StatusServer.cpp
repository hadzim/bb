/*
 * StatusServer.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#include "StatusServer.h"
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

	StatusServer::StatusServer() {
		dataHolder = new DataHolder();
	}

	StatusServer::~StatusServer() {

	}

	int StatusServer::main(const std::vector<std::string>& args) {

		std::cout << "main StatusServer.start" << std::endl;

		{

			unsigned short port = (unsigned short) config().getInt("HTTPFormServer.port", 8888);

			// set-up a server socket
			ServerSocket svs(port);
			// set-up a HTTPServer instance
			HTTPServer srv(new StatusRequestHandlerFactory(dataHolder), svs, new HTTPServerParams);

			srv.start();
			TBS::BB::Services::Data::DBus::Client::Ptr observer = new TBS::BB::Services::Data::DBus::Client();
			observer->DataDistributor().SensorDataReceived += Poco::delegate(this, &StatusServer::onData);
			observer->DataDistributor().StatusReceived += Poco::delegate(this, &StatusServer::onStatus);

			BB::ServiceNotification::serviceReady();


			this->waitForTerminationRequest();

			BB::ServiceNotification::serviceDisabled();


			observer->DataDistributor().SensorDataReceived -= Poco::delegate(this, &StatusServer::onData);
			observer->DataDistributor().StatusReceived -= Poco::delegate(this, &StatusServer::onStatus);

			srv.stop();

		}

		std::cout << "main StatusServer.stop" << std::endl;

		return EXIT_OK;
	}

	void StatusServer::onData(TBS::BB::Services::Data::IDataDistributor::SensorDataReceivedArg & arg) {
		BB::SensorData sd = BB::SensorDataHelpers::eventArg2SensorData(arg);
		dataHolder->add(sd);
	}

	void StatusServer::onStatus(TBS::BB::Services::Data::IDataDistributor::StatusReceivedArg & arg) {
		BB::RuntimeStatus s = BB::RuntimeStatus::fromShortString(arg);
		dataHolder->add(s);
	}

} /* namespace BB */
