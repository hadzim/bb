/*
 * SensorData.cpp
 *
 *  Created on: 18.3.2014
 *      Author: JV
 */

#include "BB/Sensor/SensorDataHelpers.h"
namespace BB {
TBS::BB::Services::Sensor::IDataDistributor::SensorDataReceivedArg SensorDataHelpers::sensorData2EventArg(
		const SensorData & s) {

	TBS::BB::Services::Sensor::IDataDistributor::SensorDataReceivedArg a;
	a.sensorDate = SensorData::date2string(s.getDate());
	a.sensorName = s.getName();
	a.sensorStatus = (int) s.getSensorStatus();
	a.sensorType = s.getType();
	a.sensorUnit = s.getUnit();
	a.sensorValue = s.getValue();
	a.sensorTextValue = s.getTextValue();
	return a;
}

SensorData SensorDataHelpers::eventArg2SensorData(
		const TBS::BB::Services::Sensor::IDataDistributor::SensorDataReceivedArg & a) {
	return SensorData(a.sensorType, a.sensorName, a.sensorUnit,
			SensorData::string2date(a.sensorDate),
			(SensorData::Status) a.sensorStatus, a.sensorValue, a.sensorTextValue);
}

void SensorDataHelpers::sendData(
		TBS::BB::Services::Sensor::IDataCollector & collector,
		const SensorData & s) {
			collector.SendSensorData(s.getType(), s.getName(), s.getUnit(),
			BB::SensorData::date2string(s.getDate()), s.getSensorStatus(),
			s.getValue(), s.getTextValue());
}

}
