#include "BB/DataRW.h"
#include <string>

#include <iostream>
#include "json/value.h"
#include "Poco/Net/HTMLForm.h"
#include "json/writer.h"
#include "json/reader.h"

#include "TBS/Log.h"

namespace BB {

	std::string RW::json2String(const Json::Value & v) {
		Json::FastWriter fw;
		return fw.write(v);
	}
	std::string RW::json2OneLine(const Json::Value & v) {
		std::string s = json2String(v);
		return Poco::replace(s, "\n", "");
	}
	Json::Value RW::string2json(const std::string & s) {
		Json::Reader r;
		Json::Value v;
		r.parse(s, v);
		return v;
	}

	Json::Value SensorDataRW::write(const SensorData & sensorData) {

		Json::Value v = Json::objectValue;
		v["type"] = sensorData.getType();
		v["name"] = sensorData.getName();
		v["rawName"] = sensorData.getRawName();
		v["unit"] = sensorData.getUnit();
		v["date"] = sensorData.getDateAsString();
		v["status"] = (int) sensorData.getSensorStatus();
		v["value"] = sensorData.getValue();
		v["textValue"] = sensorData.getTextValue();

		return v;
	}

	SensorData SensorDataRW::read(const Json::Value & v) {
		return SensorData(v["type"].asString(), v["name"].asString(), v["rawName"].asString(), v["unit"].asString(),
				SensorData::string2date(v["date"].asString()), (SensorData::Status) v["status"].asInt(), v["value"].asDouble(), v["textValue"].asString());
	}

	Json::Value Sensor2DataRW::write(const Sensor::Data & sensorData) {
		Json::Value v = Json::objectValue;
		v["date"] = sensorData.getDateAsString();
		v["value"] = sensorData.getValue();
		for (const auto & extraKey : sensorData.additional()) {
			v[extraKey.first] = extraKey.second;
		}
		return v;
	}
	Sensor::Data Sensor2DataRW::read(const Json::Value & v) {
		Sensor::Data val(v["value"].asDouble(), SensorData::string2date(v["date"].asString()));

		for (const std::string & mem : v.getMemberNames()) {
			if (mem == "value") {
				continue;
			}
			if (mem == "date") {
				continue;
			}
			val.set(mem, v[mem].asString());
		}
		return val;
	}

	Json::Value NodeDataRW::write(const Node::Data & sensorData) {
		Json::Value v = Json::objectValue;
		v["date"] = sensorData.getDateAsString();
		v["value"] = sensorData.getValue();
		for (const auto & extraKey : sensorData.additional()) {
			v[extraKey.first] = extraKey.second;
		}
		return v;
	}
	Node::Data NodeDataRW::read(const Json::Value & v) {
		Node::Data val(v["value"].asDouble(), Node::string2date(v["date"].asString()));

		for (const std::string & mem : v.getMemberNames()) {
			if (mem == "value") {
				continue;
			}
			if (mem == "date") {
				continue;
			}
			val.set(mem, v[mem].asString());
		}
		return val;
	}

	Json::Value StatusDataRW::write(const RuntimeStatus & status) {

		Json::Value v = Json::objectValue;
		v["date"] = SensorData::date2string(SensorData::localNow());
		v["status"] = RuntimeStatus::toShortString(status);
		return v;
	}

	RuntimeStatus StatusDataRW::read(const Json::Value & v) {
		LNOTICE("Parse")<< "parse: " << v["status"].asString() << " date " << v["date"].asString() << LE;
			LNOTICE("Parse") << RuntimeStatus::toShortString(RuntimeStatus::fromShortString(v["status"].asString(), SensorData::string2date(v["date"].asString()))) << LE;
			return RuntimeStatus::fromShortString(
					v["status"].asString(),
					SensorData::string2date(v["date"].asString())
			);
		}

	Json::Value TaskDataRW::write(const Task & task) {
		Json::Value v = Json::objectValue;

		v["date"] = SensorData::date2string(SensorData::localNow());
		v["what"] = task.getWhat();
		v["params"] = task.getParams();
		v["source"] = task.getSource();
		v["destination"] = task.getDestination();
		return v;
	}

	Task TaskDataRW::read(const Json::Value & v) {
		return Task(v["what"].asString(), v["params"].asString(), v["source"].asString(), v["destination"].asString());
	}

	Json::Value NotificationDataRW::write(const Notification & ntf) {
		Json::Value v = Json::objectValue;
		v["level"] = ntf.getLevel();
		v["source"] = ntf.getSource();
		v["message"] = ntf.getMessage();
		v["date"] = ntf.getDateAsString();

		return v;
	}
	Notification NotificationDataRW::read(const Json::Value & v) {
		return Notification((Notification::Level) v["level"].asInt(), v["source"].asString(), v["message"].asString(),
				SensorData::string2date(v["date"].asString()));
	}

}

