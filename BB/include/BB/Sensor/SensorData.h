/*
 * SensorData.h
 *
 *  Created on: 18.3.2014
 *      Author: JV
 */

#ifndef SENSORDATA_H_
#define SENSORDATA_H_

#include <string>
#include <Poco/DateTime.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeParser.h>
#include <Poco/DateTimeFormat.h>
#include <vector>

#include <iostream>

namespace BB {

class SensorData {
	public:

		static const std::string Temperature;
		static const std::string ForecastTemperature;

		static const std::string UnitTemperature;

		typedef std::vector <SensorData> List;

		enum Status {
			Sensor_Unknown = 0,
			Sensor_Error = -1,
			Sensor_Ok = 1,
		};


		SensorData(
				const std::string & type,
				const std::string & name,
				const std::string & rawName,
				const std::string & unit,
				const Poco::DateTime & date,
				const Status & status,
				const double & value,
				const std::string & textValue);

		std::string getType() const;
		std::string getName() const;
		std::string getRawName() const;
		std::string getUnit() const;
		double getValue() const;
		Status getSensorStatus() const;
		Poco::DateTime getDate() const;
		std::string getDateAsString() const;

		std::string getTextValue() const;

		static std::string date2string(const Poco::DateTime & date);
		static Poco::DateTime string2date(const std::string & date);

		static Poco::DateTime localNow();
private:

	std::string type;
	std::string name;
	std::string rawname;
	std::string unit;

	Poco::DateTime date;
	Status status;
	double value;
	std::string textValue;
};

}

std::ostream & operator<<(std::ostream & o, const BB::SensorData & s);

#endif /* SENSORDATA_H_ */
