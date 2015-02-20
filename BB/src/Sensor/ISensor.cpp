/*
 * ISensor.cpp
 *
 *  Created on: Feb 1, 2013
 *      Author: root
 */

#include "BB/Sensor/ISensor.h"

namespace BB {

	ISensor::~ISensor() {

	}

	ISensor2::~ISensor2() {
	}

	Sensor2::Sensor2(Sensor::Info i, Sensor::Data d) :
			info(i), data(d) {
	}
	Sensor::Info Sensor2::getInfo() {
		return this->info;
	}
	Sensor::Data Sensor2::getData() {
		return this->data;
	}

	SensorFactory2::SensorFactory2(int timeoutMs) :
			timeoutMs(timeoutMs) {

	}
	ISensor2::PtrList SensorFactory2::getSensors() {
		return this->sensors;
	}
	void SensorFactory2::addSensor(ISensor2::Ptr s) {
		this->sensors.push_back(s);
	}

} /* namespace TBS */
