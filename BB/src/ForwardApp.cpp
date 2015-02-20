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

#include "TBS/Log.h"

namespace BB {

	ForwarderApp::ForwarderApp(std::string name, IForwarderFactory::Ptr factory) : name(name), factory(factory){

	}

	ForwarderApp::ForwarderApp(std::string name, IForwarderFactory::Ptr factory, IForwarderFactory::Ptr factory2) : name(name), factory(factory), factory2(factory2){

	}

	ForwarderApp::~ForwarderApp() {

	}

	int ForwarderApp::main(const std::vector<std::string>& args) {

		try {
			Poco::Thread::sleep(1000);


		Poco::Thread::sleep(2000);

		TBS::initLogs(name, 6, "/tmp/");

		std::cout << "main ForwarderApp.start" << std::endl;


			this->forwarder = this->factory->createForwarder();
			if (this->factory2){
				this->forwarder2 = this->factory2->createForwarder();
			}

			this->observer = new TBS::BB::Services::Data::DBus::Client();
			this->observer->DataDistributor().SensorDataReceived += Poco::delegate(this, &ForwarderApp::onData);
			this->observer->DataDistributor().StatusReceived += Poco::delegate(this, &ForwarderApp::onStatus);
			this->observer->DataDistributor().TaskReceived += Poco::delegate(this, &ForwarderApp::onTask);
			this->observer->DataDistributor().NotificationReceived += Poco::delegate(this, &ForwarderApp::onNtf);

			BB::ServiceNotification::serviceReady();

			this->waitForTerminationRequest();

			BB::ServiceNotification::serviceDisabled();

			this->observer->DataDistributor().SensorDataReceived -= Poco::delegate(this, &ForwarderApp::onData);
			this->observer->DataDistributor().StatusReceived -= Poco::delegate(this, &ForwarderApp::onStatus);
			this->observer->DataDistributor().TaskReceived -= Poco::delegate(this, &ForwarderApp::onTask);
			this->observer->DataDistributor().NotificationReceived -= Poco::delegate(this, &ForwarderApp::onNtf);

			this->observer = NULL;

			forwarder = NULL;
			forwarder2 = NULL;

			std::cout << "main ForwarderApp.stop" << std::endl;
		} catch (Poco::Exception & e){
			std::cout << "ERR: " << e.displayText() << std::endl;
			throw;
		}

		return EXIT_OK;
	}

	void ForwarderApp::onData(TBS::BB::Services::Data::IDataDistributor::SensorDataReceivedArg & arg){
		BB::SensorData sd = BB::SensorDataHelpers::eventArg2SensorData(arg);

		Poco::Mutex::ScopedLock l(m);
		this->forwarder->forward(sd);
		if (this->forwarder2){
			this->forwarder2->forward(sd);
		}
	}

	void ForwarderApp::onStatus(TBS::BB::Services::Data::IDataDistributor::StatusReceivedArg & arg){
		BB::RuntimeStatus sd = BB::RuntimeStatus::fromShortString(arg);
		Poco::Mutex::ScopedLock l(m);

		this->cummulativeStatus.update(sd);
		this->forwarder->forward(this->cummulativeStatus);
		if (this->forwarder2){
			this->forwarder2->forward(this->cummulativeStatus);
		}
	}

	void ForwarderApp::onTask(TBS::BB::Services::Data::IDataDistributor::TaskReceivedArg & arg){
		BB::Task t(arg.what, arg.params, arg.source, arg.destination);

		LDEBUG("FWApp") << "fwd: task received " << t << LE;

		Poco::Mutex::ScopedLock l(m);
		this->forwarder->forward(t);
		if (this->forwarder2){
			this->forwarder2->forward(t);
		}
	}

	void ForwarderApp::onNtf(TBS::BB::Services::Data::IDataDistributor::NotificationReceivedArg & arg){
		BB::Notification t((BB::Notification::Level)arg.level, arg.source, arg.message);

		LDEBUG("FWApp") << "fwd: ntf received " << t << LE;

		Poco::Mutex::ScopedLock l(m);
		this->forwarder->forward(t);
		if (this->forwarder2){
			this->forwarder2->forward(t);
		}
	}



} /* namespace BB */
