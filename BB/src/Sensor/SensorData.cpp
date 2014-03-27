/*
 * SensorData.cpp
 *
 *  Created on: 18.3.2014
 *      Author: JV
 */

#include "BB/Sensor/SensorData.h"

namespace BB {

const std::string SensorData::Temperature = "Temperature";
const std::string SensorData::ForecastTemperature = "ForecastTemperature";

const std::string SensorData::UnitTemperature = "C";

SensorData::SensorData(
				const std::string & type,
				const std::string & name,
				const std::string & rawname,
				const std::string & unit,
				const Poco::DateTime & date,
				const Status & status,
				const double & value,
				const std::string & textValue) : type(type), name(name), rawname(rawname), unit(unit), date(date), status(status), value(value), textValue(textValue){

		}

		std::string SensorData::getType() const {
			return type;
		}
		std::string SensorData::getName() const {
			return name;
		}
		std::string SensorData::getRawName() const {
			return rawname;
		}
		std::string SensorData::getUnit() const {
			return unit;
		}
		double SensorData::getValue() const {
			return value;
		}
		std::string SensorData::getTextValue() const {
			return textValue;
		}
		SensorData::Status SensorData::getSensorStatus()  const{
			return status;
		}
		Poco::DateTime SensorData::getDate() const {
			return date;
		}
		std::string SensorData::getDateAsString() const {
			return date2string(date);
		}

		std::string SensorData::date2string(const Poco::DateTime & date){
			return Poco::DateTimeFormatter::format(date, Poco::DateTimeFormat::ISO8601_FORMAT);
		}
		Poco::DateTime SensorData::string2date(const std::string & date){
			int tz;
			 return Poco::DateTimeParser::parse(Poco::DateTimeFormat::ISO8601_FORMAT, date, tz);
		}

}

std::ostream & operator<<(std::ostream & o, const BB::SensorData & s){
	o 		<< s.getType() << ":" << s.getName()
			<< " at " << BB::SensorData::date2string(s.getDate())
			<< " value: " << s.getValue() << " " << s.getUnit() << " tvalue: " << s.getTextValue();
	return o;
}
