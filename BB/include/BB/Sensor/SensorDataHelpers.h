/*
 * SensorData.h
 *
 *  Created on: 18.3.2014
 *      Author: JV
 */

#ifndef SENSORDATAHELPERS_H_
#define SENSORDATAHELPERS_H_

#include "BB/Services/Sensor.h"
#include "BB/Sensor/SensorData.h"

namespace BB {
class SensorDataHelpers {

public:
	static TBS::BB::Services::Sensor::IDataDistributor::SensorDataReceivedArg sensorData2EventArg(
			const SensorData & s);

	static SensorData eventArg2SensorData (
			const TBS::BB::Services::Sensor::IDataDistributor::SensorDataReceivedArg & a);

	static void sendData( TBS::BB::Services::Sensor::IDataCollector & collector, const SensorData & s);

};
}

#endif /* SENSORDATA_H_ */
