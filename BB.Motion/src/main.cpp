/*
 * main.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#include "BB/App.h"
#include "BB/SensorApp.h"
#include "BB/Sensor/Board/MotionSensor.h"


namespace BB {

class BoardMotionFactory: public ISensorFactory {
	virtual Sensors createSensors() {
		Sensors s;

		//P8.12
		s.push_back(
				new BB::MotionSensorBBB(44)
		);

		//P8.14
		s.push_back(
				new BB::MotionSensorBBB(26)
		);

		/*
		s.push_back(
				new BB::MotionSensor(2)
		);*/
		return s;
	}
};
}

SENSOR_BB_MAIN("Motion", BB::BoardMotionFactory)

