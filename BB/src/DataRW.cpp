#include "BB/DataRW.h"
#include <string>

#include <iostream>
#include "json/value.h"
#include "Poco/Net/HTMLForm.h"
#include "json/writer.h"
#include "json/reader.h"

#include "TBS/Log.h"

namespace BB {

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
		v["tags"] = Json::arrayValue;

		for (const auto & tag : sensorData.tags()) {
			v["tags"].append(Json::Value(tag));
		}

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
			if (mem == "tags") {
				for (auto & obj : v[mem]){
					//LTRACE("parse") << "try to parse tag as string: " << obj.type() << " isstring: " << obj.isString() << LE;
					val.tags().insert(obj.asString());
				}
				continue;
			}
			val.set(mem, v[mem].asString());
		}

		//LTRACE("parse") << "try to parse done" << LE;

		return val;
	}

	Json::Value NodeSensorRW::write(const Node::Sensor & sensorData){
		Json::Value v = Json::objectValue;
		v["name"] = sensorData.name;
		v["type"] = sensorData.type;
		v["unit"] = sensorData.unit;
		return v;
	}

	Node::Sensor NodeSensorRW::read(const Json::Value & v){
		return Node::Sensor(v["name"].asString(), v["type"].asString(), v["unit"].asString());
	}

	Json::Value NodeSettingsRW::write(const Node::Setting & sensorData){
		Json::Value v = Json::objectValue;
		v["name"] = sensorData.name;
		v["type"] = sensorData.type;
		if (sensorData.defaultValue.isEmpty()){
			v["value"] = Json::nullValue;
		} else {
			v["value"] = sensorData.defaultValue.convert<std::string>();
		}

		return v;
	}

	Node::Setting NodeSettingsRW::read(const Json::Value & v){
		Node::Setting::Value val;
		if (!v["value"].isNull()){
			val = Node::Setting::Value(v["value"].asString());
		}
		return Node::Setting(v["name"].asString(), v["type"].asString(), val);
	}


	Json::Value NodeInfoRW::write(const Node::Info & info){
		Json::Value v = Json::objectValue;
		v["uid"] = info.getUID();
		v["type"] = info.getType();
		v["sensors"] = Json::arrayValue;
		v["settings"] = Json::arrayValue;

		for (const auto & s : info.getSensors()) {
			v["sensors"].append(NodeSensorRW::write(s.second));
		}
		for (const auto & s : info.getSettings()) {
			v["settings"].append(NodeSettingsRW::write(s.second));
		}

		return v;
	}
	Node::Info NodeInfoRW::read(const Json::Value & value){

		Node::Sensors::List sensors;
		for (auto & obj : value["sensors"]){
			sensors.push_back(NodeSensorRW::read(obj));
		}
		Node::Sensors s(sensors);

		Node::Setting::List settings;
		for (auto & obj : value["settings"]){
			settings.push_back(NodeSettingsRW::read(obj));
		}
		Node::Settings se(settings);

		return Node::Info(value["uid"].asString(), value["type"].asString(), s, se);
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

	Json::Value EventLogMessageRW::write(const INode::EventLogMessage & msg){
		Json::Value v(Json::objectValue);
		v["time"] = SensorData::date2string(msg.time);
		v["data"] = msg.data;
		v["dataType"] = msg.dataType;
		v["message"] = msg.message;
		v["level"] = msg.level;
		return v;
	}
	INode::EventLogMessage EventLogMessageRW::read(const Json::Value & value){
		INode::EventLogMessage msg(
				value["message"].asString(),
				value["dataType"].asString(),
				value["data"].asString(),
				value["level"].asInt()
		);
		msg.time = SensorData::string2date(value["time"].asString());
		return msg;
	}

}

