/*
 * SensorData.h
 *
 *  Created on: 18.3.2014
 *      Author: JV
 */

#ifndef NODETYPES_H_
#define NODETYPES_H_

#include <string>
#include <Poco/DateTime.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeParser.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DynamicAny.h>
#include <vector>

#include <iostream>
#include <map>
#include <set>

namespace BB {

	namespace Node {

		std::string date2string(const Poco::DateTime & date);
		Poco::DateTime string2date(const std::string & date);
		Poco::DateTime localNow();

		class Setting {
			public:
				typedef std::vector<Setting> List;
				typedef std::map<std::string, Setting> Map;

				std::string name;
				std::string type;
				Poco::DynamicAny defaultValue;

				Setting(std::string n, std::string t = "text", Poco::DynamicAny defaultValue = Poco::DynamicAny()) :
						name(n), type(t), defaultValue(defaultValue) {

				}
		};

		class DataStream {
			public:
				typedef std::vector<DataStream> List;
				typedef std::map<std::string, DataStream> Map;
				std::string name;
				std::string type;
				std::string unit;

				DataStream(std::string n, std::string t = "text", std::string unit = "") :
						name(n), type(t), unit(unit) {
				}
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


		template <class InnerClass>
		class Collection {
			public:
				typedef typename InnerClass::List List;
				typedef typename InnerClass::Map Map;
				typedef std::string Key;
				typedef std::set<Key> Keys;

				Collection(const List & data = List()) {
					for (const auto & val : data) {
						map.insert(std::make_pair(val.name, val));
						allKeys.insert(val.name);
					}
				}
				const InnerClass & get(Key key) const {
					if (map.find(key) == map.end()) {
						throw Poco::Exception("No Key " + key + " in map");
					}
					return map.at(key);
				}
				bool has(Key key) const {
					return (map.find(key) != map.end());
				}
				const Keys & keys() const {
					return allKeys;
				}
			private:
				Map map;
				Keys allKeys;
		};

		typedef Collection<DataStream> DataStreams;
		typedef Collection<Setting> Settings;

		class Info {
			public:
				static const std::string Temperature;
				static const std::string ForecastTemperature;
				static const std::string Motion;
				static const std::string Camera;
				static const std::string Status;

				Info(std::string uid, std::string type, const DataStreams & dataStreams = DataStreams(), const Settings & settings = Settings());
				Info(std::string uid, std::string type, const Settings & settings);

				std::string getUID() const;

				std::string getType() const;
				std::string getUnit() const;
				const Settings & getSettings() const;
				const DataStreams & getDataStreams() const;
			private:
				std::string uid;
				std::string type;
				DataStreams dataStreams;
				Settings settings;
		};



/*
		struct SettingValue {
				Poco::DynamicAny value;
				Poco::DynamicAny newvalue;
		};


		//TODO
		class Status {
			public:

		};*/
	}
}
/*
std::ostream & operator<<(std::ostream & o, const BB::Sensor::Data & s);
std::ostream & operator<<(std::ostream & o, const BB::Sensor::Info & s);
std::ostream & operator<<(std::ostream & o, const BB::Sensor::Setting & s);
std::ostream & operator<<(std::ostream & o, const BB::Sensor::Status & s);
*/
#endif /* SENSORDATA_H_ */
