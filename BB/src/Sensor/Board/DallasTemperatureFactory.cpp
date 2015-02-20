/*
 * DallasTemperatureSensor.cpp
 *
 *  Created on: Feb 3, 2013
 *      Author: root
 */

#include <BB/Sensor/SensorData2.h>
#include "BB/Sensor/Board/DallasTemperatureFactory.h"
#include <set>
#include "Poco/Glob.h"
#include "Poco/File.h"
#include <iostream>
#include "Poco/Path.h"
#include <fstream>
#include <sstream>
#include <stdio.h>
#include "BB/Configuration.h"

#include "TBS/Log.h"

namespace BB {

	DallasTemperatureFactory::DallasTemperatureFactory() {

	}

	DallasTemperatureFactory::~DallasTemperatureFactory() {

	}

	ISensor2::PtrList DallasTemperatureFactory::getSensors(){
		ISensor2::PtrList sensors;

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

			Sensor::Info info(sensorID, Sensor::Info::Temperature, Sensor::Info::UnitTemperature);
			Sensor::Data data(val, Sensor::localNow());

			sensors.push_back(new Sensor2(info, data));
			LINFO("Temperature") << "Read: info " << info << " data: " << data << LE;

			std::cout << "sensor: " << sensorID  << " " << val << std::endl;

		}

		return sensors;
	}
	int DallasTemperatureFactory::getPeriodInMs(){
		return 1000*60 * 2;
	}
} /* namespace BB */
