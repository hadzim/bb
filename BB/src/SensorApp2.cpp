/*
 * SensorApp.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#include <BB/ServiceNotification.h>
#include "BB/SensorApp2.h"
#include <iostream>
#include <Poco/Delegate.h>
#include "Poco/Thread.h"
#include "BB/Sensor/Net/Forecast.h"
#include "BB/Sensor/SensorDataHelpers.h"
#include "BB/Services/DataSvc_DBus.h"

#include "TBS/Log.h"

namespace BB {

	SensorApp2::SensorApp2(std::string name, ISensorFactory2::Ptr factory) :
			name(name), factory(factory) {


	}

	SensorApp2::~SensorApp2() {
		if (timer){
			timer->Timer -= Poco::delegate(this, &SensorApp2::onTimer);
			timer->stop();
		}

		dataSender = NULL;
	}

	int SensorApp2::main(const std::vector<std::string>& args) {

		Poco::Thread::sleep(1000);

		std::cout << "enter main: " << name << std::endl;

		TBS::initLogs(name, 8, "/tmp/");

		dataSender = IDataSender2::create(name, "bb@default");

		timer = new TBS::SimpleTimer();
		timer->Timer += Poco::delegate(this, &SensorApp2::onTimer);
		timer->start(factory->getPeriodInMs() / 20, factory->getPeriodInMs());


		std::cout << "main SensorApp2.start" << std::endl;

		/*SensorExecutor::PtrList sensors;

		ISensorFactory::Sensors creating = factory->createSensors();
		for (ISensorFactory::Sensors::iterator i = creating.begin(); i != creating.end(); i++) {
			SensorExecutor::Ptr s = new SensorExecutor(*i);
			sensors.push_back(s);
		}
		//events in
		for (SensorExecutor::PtrList::iterator s = sensors.begin(); s != sensors.end(); s++) {
			(*s)->RequestsReady += Poco::delegate(this, &SensorApp::onRequest);
		}
		*/
		BB::ServiceNotification::serviceReady();

		this->waitForTerminationRequest();

		BB::ServiceNotification::serviceDisabled();
/*
		//events out
		for (SensorExecutor::PtrList::iterator s = sensors.begin(); s != sensors.end(); s++) {
			(*s)->RequestsReady -= Poco::delegate(this, &SensorApp::onRequest);
		}

		sensors.clear();
*/
		std::cout << "main SensorApp2.stop" << std::endl;

		return EXIT_OK;
	}
	void SensorApp2::onTimer(TBS::SimpleTimer::TimerArg & t){
		try {

			std::string bb = "bb@default";

			std::cout << "timer ticked in app : " << name << std::endl;
			auto sensors = factory->getSensors();
			for (auto & sensor : sensors){
				//publish on mosquitto
				auto info = sensor->getInfo();
				std::cout << "sensor: " << sensor->getInfo().getUID() << " value: " << sensor->getData().getValue() << std::endl;

				dataSender->send(info);
				dataSender->send(info.getUID(), sensor->getData());

			}
		} catch (Poco::Exception & e){
			LERROR("Sensor.App") << e.displayText() << LE;
		}
	}
/*
	void SensorApp::onRequest(ISensor::Ptr & t) {
		ISensor::Ptr sp = t;
		std::cout << "timer ticked for sensor " << sp->getName() << std::endl;

		ISensor::Requests reqs = sp->getRequests();
		for (ISensor::Requests::iterator r = reqs.begin(); r != reqs.end(); r++) {
			BB::SensorDataHelpers::sendData(client->DataCollector(), *r);
		}
	}
	*/
} /* namespace BB */
