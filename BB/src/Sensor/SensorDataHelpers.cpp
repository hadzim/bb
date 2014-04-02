/*
 * SensorData.cpp
 *
 *  Created on: 18.3.2014
 *      Author: JV
 */

#include "BB/Sensor/SensorDataHelpers.h"
namespace BB {
namespace SensorDataHelpers {

TBS::BB::Services::Sensor::IDataDistributor::SensorDataReceivedArg sensorData2EventArg(
		const SensorData & s) {

	TBS::BB::Services::Sensor::IDataDistributor::SensorDataReceivedArg a;
	a.sensorDate = SensorData::date2string(s.getDate());
	a.sensorName = s.getName();
	a.sensorRawName = s.getRawName();
	a.sensorStatus = (int) s.getSensorStatus();
	a.sensorType = s.getType();
	a.sensorUnit = s.getUnit();
	a.sensorValue = s.getValue();
	a.sensorTextValue = s.getTextValue();
	return a;
}

SensorData eventArg2SensorData(
		const TBS::BB::Services::Sensor::IDataDistributor::SensorDataReceivedArg & a) {
	return SensorData(a.sensorType, a.sensorName, a.sensorRawName, a.sensorUnit,
			SensorData::string2date(a.sensorDate),
			(SensorData::Status) a.sensorStatus, a.sensorValue,
			a.sensorTextValue);
}

void sendData(TBS::BB::Services::Sensor::IDataCollector & collector,
		const SensorData & s) {
	collector.SendSensorData(s.getType(), s.getName(), s.getRawName(),
			s.getUnit(), BB::SensorData::date2string(s.getDate()),
			s.getSensorStatus(), s.getValue(), s.getTextValue());
}

std::string sensorID(const std::string & sensorType,
		const std::string & sensorName) {
	return "<" + sensorType + "::" + sensorName + ">";
}
std::string sensorID(const SensorData & s) {
	return sensorID(s.getType(), s.getName());
}

std::string sensorID(const TBS::BB::WebUI::SensorInfo & i){
	return sensorID(i.sensorType, i.sensorName);
}

}
}
