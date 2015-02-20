/*
 * SensorData.h
 *
 *  Created on: 18.3.2014
 *      Author: JV
 */

#ifndef SENSORDATA2_H_
#define SENSORDATA2_H_

#include <string>
#include <Poco/DateTime.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeParser.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DynamicAny.h>
#include <vector>

#include <iostream>
#include <map>

namespace BB {

	namespace Sensor {

		std::string date2string(const Poco::DateTime & date);
		Poco::DateTime string2date(const std::string & date);
		Poco::DateTime localNow();

		class Setting {
			public:
				typedef std::vector <Setting> List;
				std::string name;
				std::string type;
				Poco::DynamicAny defaultValue;

				Setting(std::string n, std::string t = "text", Poco::DynamicAny defaultValue = Poco::DynamicAny()) : name(n), type(t), defaultValue(defaultValue){

				}
		};

		struct SettingValue {
				Poco::DynamicAny value;
				Poco::DynamicAny newvalue;
		};

		typedef Setting::List Settings;

		class Info {
			public:
				static const std::string Temperature;
				static const std::string ForecastTemperature;
				static const std::string Motion;
				static const std::string Camera;
				static const std::string Status;

				static const std::string UnitTemperature;

				Info(std::string uid, std::string type, const Settings & settings = Settings());
				Info(std::string uid, std::string type, std::string unit, const Settings & settings = Settings());

				std::string getUID() const;

				std::string getType() const;
				std::string getUnit() const;
				const Settings & getSettings() const;
			private:
				std::string uid;
				std::string type;
				std::string unit;
				Settings settings;
		};

		class Data {
			public:
				static const std::string KeyValue;
				static const std::string KeyDate;

				Data(const double & value, const Poco::DateTime & date);

				double getValue() const;
				Poco::DateTime getDate() const;
				std::string getDateAsString() const;

				std::string get(std::string key) const;
				void set(std::string key, std::string value);
				bool has(std::string key) const;

				typedef std::map<std::string, std::string> Values;
				const Values & additional() const;
			private:
				double value;
				Poco::DateTime date;
				Values additionalInfo;
		};

		//TODO
		class Status {
			public:

		};
	}
}

std::ostream & operator<<(std::ostream & o, const BB::Sensor::Data & s);
std::ostream & operator<<(std::ostream & o, const BB::Sensor::Info & s);
std::ostream & operator<<(std::ostream & o, const BB::Sensor::Setting & s);
std::ostream & operator<<(std::ostream & o, const BB::Sensor::Status & s);

#endif /* SENSORDATA_H_ */
