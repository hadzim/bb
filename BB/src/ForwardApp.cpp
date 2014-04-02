/*
 * ForwarderApp.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#include <BB/ServiceNotification.h>
#include "BB/ForwardApp.h"
#include <iostream>
#include <Poco/Delegate.h>
#include "Poco/Thread.h"
#include "BB/Sensor/SensorDataHelpers.h"

namespace BB {

	ForwarderApp::ForwarderApp(IForwarderFactory::Ptr factory) : factory(factory){

	}

	ForwarderApp::~ForwarderApp() {

	}

	int ForwarderApp::main(const std::vector<std::string>& args) {


		Poco::Thread::sleep(1000);

		std::cout << "main ForwarderApp.start" << std::endl;

		this->forwarder = this->factory->createForwarder();

		this->observer = new TBS::BB::Services::Sensor::DBus::Client();
		this->observer->DataDistributor().SensorDataReceived += Poco::delegate(this, &ForwarderApp::onData);

		BB::ServiceNotification::serviceReady();

		this->waitForTerminationRequest();

		BB::ServiceNotification::serviceDisabled();

		this->observer->DataDistributor().SensorDataReceived -= Poco::delegate(this, &ForwarderApp::onData);
		this->observer = NULL;

		forwarder = NULL;

		std::cout << "main ForwarderApp.stop" << std::endl;

		return EXIT_OK;
	}

	void ForwarderApp::onData(TBS::BB::Services::Sensor::IDataDistributor::SensorDataReceivedArg & arg){
		Poco::Mutex::ScopedLock l(m);
		BB::SensorData sd = BB::SensorDataHelpers::eventArg2SensorData(arg);

		std::cout << "fwd data: " << sd << std::endl;

		this->forwarder->forward(sd);
	}



} /* namespace BB */
