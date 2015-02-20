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
		const std::string Info::Camera = "Camera";
		const std::string Info::Status = "Status";

		Info::Info(std::string uid, std::string type, const Settings & settings) :
				uid(uid), type(type), settings(settings) {
		}
		Info::Info(std::string uid, std::string type, const DataStreams & dataStreams, const Settings & settings) :
				uid(uid), type(type), dataStreams(dataStreams), settings(settings) {

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

		const DataStreams & Info::getDataStreams() const {
			return this->dataStreams;
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

		const Data::Values & Data::additional() const {
			return this->additionalInfo;
		}
	}
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
