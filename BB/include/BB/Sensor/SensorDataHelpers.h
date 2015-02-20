/*
 * SensorData.h
 *
 *  Created on: 18.3.2014
 *      Author: JV
 */

#ifndef SENSORDATAHELPERS_H_
#define SENSORDATAHELPERS_H_

#include "BB/Services/Data.h"
#include "BB/Sensor/SensorData.h"
#include "BB/Services/WebUI.h"
namespace BB {
namespace SensorDataHelpers {


	TBS::BB::Services::Data::IDataDistributor::SensorDataReceivedArg sensorData2EventArg(
			const SensorData & s);

	SensorData eventArg2SensorData (
			const TBS::BB::Services::Data::IDataDistributor::SensorDataReceivedArg & a);

	void sendData( TBS::BB::Services::Data::IDataCollector & collector, const SensorData & s);

	std::string sensorID(const std::string & sensorType, const std::string & sensorName);
	std::string sensorID(const SensorData & s);
	std::string sensorID(const TBS::BB::WebUI::SensorInfo & i);

	std::string sensorRawID(const SensorData & s);
	std::string sensorRawID(const TBS::BB::WebUI::SensorInfo & i);


}
}

#endif /* SENSORDATA_H_ */
