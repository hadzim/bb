/*
 * main.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#include "BB/App.h"
#include "BB/SensorApp.h"
#include "BB/Sensor/Board/DallasTemperatureSensor.h"


namespace BB {

class BoardTemperatureFactory: public ISensorFactory {
	virtual Sensors createSensors() {
		Sensors s;
		s.push_back(
				new BB::DallasTemperatureSensor()
		);
		return s;
	}
};
}

SENSOR_BB_MAIN(BB::BoardTemperatureFactory)

