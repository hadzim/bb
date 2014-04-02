#include "BB/Sensor/SensorDataRW.h"
#include <string>

#include <iostream>
#include "json/value.h"
#include "Poco/Net/HTMLForm.h"
#include "json/writer.h"
#include "json/reader.h"

namespace BB {

	Json::Value SensorDataRW::write2Json(const SensorData & sensorData) {

		Json::Value v = Json::objectValue;
		v["type"] = sensorData.getType();
		v["name"] = sensorData.getName();
		v["rawName"] = sensorData.getRawName();
		v["unit"] = sensorData.getUnit();
		v["date"] = sensorData.getDateAsString();
		v["status"] = (int)sensorData.getSensorStatus();
		v["value"] = sensorData.getValue();
		v["textValue"] = sensorData.getTextValue();

		return v;
	}

	SensorData SensorDataRW::read(const Json::Value & v){
		return SensorData(
				v["type"].asString(),
				v["name"].asString(),
				v["rawName"].asString(),
				v["unit"].asString(),
				SensorData::string2date(v["date"].asString()),
				(SensorData::Status)v["status"].asInt(),
				v["value"].asDouble(),
				v["textValue"].asString()
		);
	}

	std::string SensorDataRW::write2String(const SensorData & sensorData) {
		Json::Value v = write2Json(sensorData);
		Json::FastWriter fw;
		return fw.write(v);
	}
	std::string SensorDataRW::write2OneLine(const SensorData & sensorData){
		std::string s = write2String(sensorData);
		return Poco::replace(s, "\n", "");
	}

	 SensorData SensorDataRW::read(const std::string & s){
		Json::Value v;
		Json::Reader r;
		r.parse(s, v);
		return read(v);
	}
}

