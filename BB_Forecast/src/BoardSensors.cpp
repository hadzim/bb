/*
 * BoardSensors.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#include "BoardSensors.h"
#include <iostream>
#include <Poco/Delegate.h>
#include "Poco/Thread.h"
#include "BB/Sensor/Net/Forecast.h"
#include "BB/Sensor/SensorDataHelpers.h"

#include "BB/Services/SensorSvc_DBus.h"


namespace BB {

	BoardSensors::BoardSensors(){
		this->addSensor(new Forecast("Josefuv Dul", "http://www.yr.no/place/Czech_Republic/Liberec/Josef%C5%AFv_D%C5%AFl/"));
		//this->addSensor(new DallasTemperatureSensor());
	}

	BoardSensors::~BoardSensors() {

	}

	void BoardSensors::addSensor(ISensor::Ptr sensor){
		SensorData data;
		data.sensor = sensor;
		data.timer = new TBS::SafeTimer(sensor->getName(), sensor->getPeriodInMs() / 10, sensor->getPeriodInMs());
		sensors.push_back(data);
	}

	int BoardSensors::main(const std::vector<std::string>& args) {

		Poco::Thread::sleep(1000);


		this->config().setString("pokus", "hodnota");

		std::cout << "main BoardSensors.start" << std::endl;

		//start periodic timers
		for (SensorData::List::iterator i = sensors.begin(); i != sensors.end(); i++){
			i->timer->start(TBS::TimerCallback<BoardSensors>(*this, &BoardSensors::onTimer));
		}

		waitForTerminationRequest();

		sensors.clear();

		return EXIT_OK;
	}

	void BoardSensors::onTimer(TBS::SafeTimer& t){
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
