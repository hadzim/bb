/*
 * SensorData.h
 *
 *  Created on: 18.3.2014
 *      Author: JV
 */

#ifndef SENSORDATARW_H_
#define SENSORDATARW_H_

#include <string>

#include <iostream>
#include "json/value.h"
#include "BB/Sensor/SensorData.h"

namespace BB {

	class SensorDataRW {
		public:
			static Json::Value write2Json(const SensorData & sensorData);
			static SensorData read(const Json::Value & value);

			static std::string write2String(const SensorData & sensorData);
			static std::string write2OneLine(const SensorData & sensorData);

			static SensorData read(const std::string &);

	};

}



#endif /* SENSORDATA_H_ */
