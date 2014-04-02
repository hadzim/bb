/*
 * main.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#include "BB/App.h"
#include "BB/SensorApp.h"
#include "BB/Sensor/Net/Forecast.h"
#include "BB/Configuration.h"

#include <Poco/Random.h>

namespace BB {

Poco::Random rand;

class VirtualTempSensor: public ISensor {
public:
	VirtualTempSensor(std::string name, int from, int interval) : name(name), from(from), max(interval){}

	virtual Requests getRequests() {
		Requests r;
		{
			int val = rand.next(max);

			SensorData data(
					SensorData::Temperature,
					Configuration::getSensorName(SensorData::Temperature, name),
					name,
					SensorData::UnitTemperature,
					Poco::DateTime(),
					SensorData::Sensor_Ok,
					val + from,
					""
			);
			r.push_back(data);
		}
		return r;
	}
	virtual int getPeriodInMs() {
		return 1000 * 10;
	}
	virtual std::string getName() {
		return name;
	}

private:
	std::string name;
	int from;
	int max;
};

class VirtualTemperatureFactory: public ISensorFactory {
	virtual Sensors createSensors() {
		Sensors s;
		s.push_back(
				new VirtualTempSensor("Outside", -12, 25)
		);
		s.push_back(
				new VirtualTempSensor("Inside", -1, 30)
		);
		s.push_back(
				new VirtualTempSensor("Heating", 5, 70)
		);
		s.push_back(
				new VirtualTempSensor("Hall", -5, 10)
		);
		return s;
	}
};
}

SENSOR_BB_MAIN(BB::VirtualTemperatureFactory)

