/*
 * SensorApp.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#include <BB/ServiceNotification.h>
#include "BB/SensorApp.h"
#include <iostream>
#include <Poco/Delegate.h>
#include "Poco/Thread.h"
#include "BB/Sensor/Net/Forecast.h"
#include "BB/Sensor/SensorDataHelpers.h"

#include "BB/Services/SensorSvc_DBus.h"


namespace BB {

	SensorApp::SensorApp(ISensorFactory::Ptr factory) : factory(factory){

	}

	SensorApp::~SensorApp() {

	}

	void SensorApp::addSensor(ISensor::Ptr sensor){
		SensorData data;
		data.sensor = sensor;
		data.timer = new TBS::SafeTimer(sensor->getName(), sensor->getPeriodInMs() / 10, sensor->getPeriodInMs());
		sensors.push_back(data);
	}

	int SensorApp::main(const std::vector<std::string>& args) {

		Poco::Thread::sleep(500);


		//this->config().setString("pokus", "hodnota");

		std::cout << "main SensorApp.start" << std::endl;

		ISensorFactory::Sensors creating = factory->createSensors();
		for (ISensorFactory::Sensors::iterator i = creating.begin(); i != creating.end(); i++){
			this->addSensor(*i);
		}

		//start periodic timers
		for (SensorData::List::iterator i = sensors.begin(); i != sensors.end(); i++){
			i->timer->start(TBS::TimerCallback<SensorApp>(*this, &SensorApp::onTimer));
		}

		BB::ServiceNotification::serviceReady();

		this->waitForTerminationRequest();

		BB::ServiceNotification::serviceDisabled();

		sensors.clear();

		std::cout << "main SensorApp.stop" << std::endl;

		return EXIT_OK;
	}

	void SensorApp::onTimer(TBS::SafeTimer& t){
		std::cout << "timer ticked" << std::endl;
		for (SensorData::List::iterator i = sensors.begin(); i != sensors.end(); i++){
			std::cout << "timer ticked for sensor " << i->sensor->getName() << std::endl;
			if (i->timer.get() == &t){


				TBS::BB::Services::Sensor::DBus::Client::Ptr client = new TBS::BB::Services::Sensor::DBus::Client();

				ISensor::Requests reqs = i->sensor->getRequests();
				for (ISensor::Requests::iterator r = reqs.begin(); r != reqs.end(); r++){
					BB::SensorDataHelpers::sendData(client->DataCollector(), *r);
				}

			}
		}
		//BeagleBone::led::get(LED)->on();
		Poco::Thread::sleep(200);
		//BeagleBone::led::get(LED)->off();

	}
} /* namespace BB */
