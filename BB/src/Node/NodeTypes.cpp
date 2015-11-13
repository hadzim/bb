/*
 * SensorData.cpp
 *
 *  Created on: 18.3.2014
 *      Author: JV
 */

#include "BB/Node/NodeTypes.h"
#include <Poco/Exception.h>
namespace BB {

	namespace Node {

		const std::string Info::Temperature = "Temperature";
		const std::string Info::ForecastTemperature = "ForecastTemperature";
		const std::string Info::Motion = "Motion";
		const std::string Info::Switch = "Switch";
		const std::string Info::Contact = "Contact";
		const std::string Info::Camera = "Camera";
		const std::string Info::Status = "Status";
		const std::string Info::Screen = "Screen";

		Info::Info(std::string uid, std::string type, const Settings & settings) :
				uid(uid), type(type), settings(settings) {
			mandatoryItems();
		}
		Info::Info(std::string uid, std::string type, const Sensors & sensors, const Settings & settings) :
				uid(uid), type(type), sensors(sensors), settings(settings) {
			mandatoryItems();
		}
		void Info::mandatoryItems(){
			if (!settings.has("name")){
				settings.insert(Setting("name"));
			}
			if (!settings.has("place")){
				settings.insert(Setting("place"));
			}
		}


		std::string Info::getUID() const {
			return this->uid;
		}

		std::string Info::getType() const {
			return this->type;
		}

		const Settings & Info::getSettings() const {
			return this->settings;
		}

		const Sensors & Info::getSensors() const {
			return this->sensors;
		}

		std::string date2string(const Poco::DateTime & date) {
			return Poco::DateTimeFormatter::format(date, Poco::DateTimeFormat::ISO8601_FORMAT);
		}
		Poco::DateTime string2date(const std::string & date) {
			int tz;
			return Poco::DateTimeParser::parse(Poco::DateTimeFormat::ISO8601_FORMAT, date, tz);
		}
		Poco::DateTime localNow() {
			Poco::LocalDateTime ldt;
			return Poco::DateTime(ldt.year(), ldt.month(), ldt.day(), ldt.hour(), ldt.minute(), ldt.second());
		}

		const std::string Data::KeyValue = "value";
		const std::string Data::KeyDate = "date";

		Data::Data(const double & value, const Poco::DateTime & date) :
				value(value), date(date) {

		}

		double Data::getValue() const {
			return this->value;
		}
		Poco::DateTime Data::getDate() const {
			return this->date;
		}
		std::string Data::getDateAsString() const {
			return date2string(this->date);
		}

		std::string Data::get(std::string key) const {
			if (additionalInfo.find(key) == additionalInfo.end()) {
				throw Poco::Exception("Non existing key in data: " + key);
			}
			return this->additionalInfo.at(key);
		}
		void Data::set(std::string key, std::string value) {
			this->additionalInfo[key] = value;
		}

		bool Data::has(std::string key) const {
			return additionalInfo.find(key) != additionalInfo.end();
		}

		const Data::Tags & Data::tags() const {
			return this->tagSet;
		}
		Data::Tags & Data::tags(){
			return this->tagSet;
		}

		const Data::Values & Data::additional() const {
			return this->additionalInfo;
		}
		void Data::setValue(double val){
			this->value = val;
		}


	}
}

std::ostream & operator<<(std::ostream & o, const BB::Node::Data & s){
	o << "val: " << s.getValue() << ", date: " << s.getDateAsString() << " tags:" << s.tags().size() << " extra: " << s.additional().size();
	return o;
}
std::ostream & operator<<(std::ostream & o, const BB::Node::Sensor & s){
	o << "name: " <<  s.name << ", type: " << s.type << ", unit: " << s.unit;
	return o;
}
std::ostream & operator<<(std::ostream & o, const BB::Node::Info & s){
	o << "info: " << s.getUID() << " type: " << s.getType() << " sensors:(";

	for (const auto & sen : s.getSensors()){
		o << "sensor: " << sen.second << " | ";
	}

	o << ")";
	return o;
}

/*
 std::ostream & operator<<(std::ostream & o, const BB::Sensor::Info & s) {
 o << "uid: " << s.getUID() << " type: " << s.getType() << " unit: " << s.getUnit();
 return o;
 }

 std::ostream & operator<<(std::ostream & o, const BB::Sensor::Status & s) {
 o << "status: ???";
 return o;
 }

 std::ostream & operator<<(std::ostream & o, const BB::Sensor::Data & s) {
 o << "val: " << s.getValue() << " date: " << s.getDateAsString();
 return o;
 }
 std::ostream & operator<<(std::ostream & o, const BB::Sensor::Setting & s){
 o << "name:" << s.name << " type: " << s.type;
 return o;
 }
 */
