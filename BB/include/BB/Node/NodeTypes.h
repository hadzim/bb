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

				typedef std::map<std::string, std::string> MetaInfo;

				typedef Poco::DynamicAny Value;

				std::string name;
				std::string type;
				Value defaultValue;
				MetaInfo metaInfo;


				Setting(std::string n, std::string t = "text", Value defaultValue = Value(), MetaInfo metaInfo = MetaInfo()) :
						name(n), type(t), defaultValue(defaultValue), metaInfo(metaInfo) {

				}
		};

		class Sensor {
			public:
				typedef std::vector<Sensor> List;
				typedef std::map<std::string, Sensor> Map;
				std::string name;
				std::string type;
				std::string unit;

				Sensor(std::string n, std::string t = "text", std::string unit = "") :
						name(n), type(t), unit(unit) {
				}
		};

		class Data {
			public:
				static const std::string KeyValue;
				static const std::string KeyDate;

				typedef std::string Tag;
				typedef std::set <Tag> Tags;

				Data(const double & value, const Poco::DateTime & date);

				void setValue(double val);
				double getValue() const;
				Poco::DateTime getDate() const;
				std::string getDateAsString() const;

				std::string get(std::string key) const;
				void set(std::string key, std::string value);
				bool has(std::string key) const;

				const Tags & tags() const;
				Tags & tags();

				typedef std::map<std::string, std::string> Values;
				const Values & additional() const;
			private:
				double value;
				Poco::DateTime date;
				Values additionalInfo;
				Tags tagSet;
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
						insert(val);
					}
				}
				const InnerClass & at(Key key) const {
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

				typedef typename Map::const_iterator const_iterator;

				const_iterator begin() const{
					return map.begin();
				}
				const_iterator end() const{
					return map.end();
				}

				void insert(const InnerClass & val){
					map.insert(std::make_pair(val.name, val));
					allKeys.insert(val.name);
				}

			private:
				Map map;
				Keys allKeys;
		};

		typedef Collection<Sensor> Sensors;
		typedef Collection<Setting> Settings;

		class Info {
			public:
				static const std::string Temperature;
				static const std::string ForecastTemperature;
				static const std::string Motion;
				static const std::string Contact;
				static const std::string Camera;
				static const std::string Status;
				static const std::string Screen;
				static const std::string Switch;

				Info(std::string uid, std::string type, const Sensors & sensors = Sensors(), const Settings & settings = Settings());
				Info(std::string uid, std::string type, const Settings & settings);

				std::string getUID() const;

				std::string getType() const;
				const Settings & getSettings() const;
				const Sensors & getSensors() const;
			private:
				void mandatoryItems();
			private:
				std::string uid;
				std::string type;
				Sensors sensors;
				Settings settings;
		};


		template <typename T>
		bool isFilled(const Poco::DynamicAny & val, T & filledVal) {
			if (val.isEmpty())
				return false;
			try {
				filledVal = val.convert<T>();
				return true;
			} catch (...) {
				return false;
			}
		}


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

std::ostream & operator<<(std::ostream & o, const BB::Node::Data & s);
std::ostream & operator<<(std::ostream & o, const BB::Node::Sensor & s);
std::ostream & operator<<(std::ostream & o, const BB::Node::Info & s);

/*
std::ostream & operator<<(std::ostream & o, const BB::Sensor::Data & s);
std::ostream & operator<<(std::ostream & o, const BB::Sensor::Info & s);
std::ostream & operator<<(std::ostream & o, const BB::Sensor::Setting & s);
std::ostream & operator<<(std::ostream & o, const BB::Sensor::Status & s);
*/
#endif /* SENSORDATA_H_ */
