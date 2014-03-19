/*
 * DallasTemperatureSensor.cpp
 *
 *  Created on: Feb 3, 2013
 *      Author: root
 */

#include "BB/Sensor/Board/DallasTemperatureSensor.h"
#include <set>
#include "Poco/Glob.h"
#include "Poco/File.h"
#include <iostream>
#include "Poco/Path.h"
#include <fstream>
#include <sstream>
#include <stdio.h>

namespace BB {

	DallasTemperatureSensor::DallasTemperatureSensor() {

	}

	DallasTemperatureSensor::~DallasTemperatureSensor() {

	}

	DallasTemperatureSensor::Requests DallasTemperatureSensor::getRequests(){

		DallasTemperatureSensor::Requests requests;

		std::set<std::string> files;
		Poco::Glob::glob("/sys/bus/w1/devices/28*", files);

		for (std::set<std::string>::iterator it = files.begin(); it != files.end(); ++it){
			Poco::Path path(*it);

			std::string sensorID = path[path.depth()-1];

			//std::cout << "sensor: " << sensorID  << std::endl;
			path.append("w1_slave");

			std::ifstream t(path.toString().c_str());

			std::string last;
			while(t.good()){
				t >> last;
			}

			//std::cout << "sensor: " << sensorID  << " " << last << std::endl;
			int temp = 0;
			if (sscanf(last.c_str(), "t=%d", &temp) != 1){
				throw Poco::Exception("Cannot parse temperature");
			}
			double val = temp * 1.0 / 1000.0;
/*
			TBS::TSeries::IPC::SensorDataStoreRequest r;
			r.setSensorName(sensorID);
			r.setSensorStatus(TBS::TSeries::IPC::SensorDataStoreRequest::Sensor_Ok);
			r.setSensorType("Temp");
			r.setSensorUnit("C");
			r.setSensorValue(val);
*/
			std::cout << "sensor: " << sensorID  << " " << val << std::endl;

			//requests.push_back(r);
		}

		return requests;

	}
	int DallasTemperatureSensor::getPeriodInMs(){
		return 1000*60*5;
	}
	std::string DallasTemperatureSensor::getName(){
		return "BoardTemperature";
	}

} /* namespace BB */
