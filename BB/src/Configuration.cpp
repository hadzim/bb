/*
 * Configuration.cpp
 *
 *  Created on: Feb 21, 2013
 *      Author: root
 */

#include "BB/Configuration.h"
#include <fstream>
#include "Poco/File.h"
#include "json/value.h"
#include "json/writer.h"
#include "json/reader.h"
#include <fstream>

#include <Poco/NumberFormatter.h>
#include <Poco/NumberParser.h>

namespace BB {
	typedef std::map<std::string, std::string> Properties;
	typedef Properties SensorProperties;

	typedef std::map<std::string, SensorProperties> TypeSensorProperties;
	typedef std::map<std::string, TypeSensorProperties> AllSensorProperties;

	typedef std::map<std::string, Properties> GlobalProperties;

	const std::string cfgName = "configuration.json";

	class ConfigurationImpl {
		private:
			GlobalProperties global;
			AllSensorProperties all;
		public:
			ConfigurationImpl() {
				load();
			}

			std::string getProperty(std::string type, std::string name, std::string property) {
				return all.at(type).at(name).at(property);
			}
			void setProperty(std::string type, std::string name, std::string property, std::string value) {
				all[type][name][property] = value;
				save();
			}
			Configuration::Property::List getProperties(const std::string type, const std::string & name){
				Configuration::Property::List l;
				for (Properties::iterator i =  all[type][name].begin(); i != all[type][name].end(); i++){
					Configuration::Property p;
					p.propName = i->first;
					p.propValue = i->second;
					l.push_back(p);
				}
				return l;
			}

			std::string getGlobalProperty(std::string name, std::string property) {
				return global.at(name).at(property);
			}
			void setGlobalProperty(std::string name, std::string property, std::string value) {
				global[name][property] = value;
				save();
			}

		private:
			void load() {
				try {
					Json::Value root;
					{
						std::ifstream istr(cfgName.c_str());
						Json::Reader r;
						r.parse(istr, root);
					}
					if (root.isObject()) {

						const Json::Value & sensorRoot = root["sensor"];
						const Json::Value & globalRoot = root["global"];

						if (sensorRoot.isArray()) {
							const Json::Value & root2 = sensorRoot;
							for (Json::Value::const_iterator a = root2.begin(); a != root2.end(); a++) {
								try {
									const Json::Value & property = *a;
									all[property["type"].asString()][property["name"].asString()][property["property"].asString()] =
											property["value"].asString();
								} catch (...) {

								}
							}
						}

						if (globalRoot.isArray()) {
							const Json::Value & root2 = globalRoot;
							for (Json::Value::const_iterator a = root2.begin(); a != root2.end(); a++) {
								try {
									const Json::Value & property = *a;
									global[property["name"].asString()][property["property"].asString()] =
											property["value"].asString();
								} catch (...) {

								}
							}
						}

					}
				} catch (...) {

				}
			}
			void save() {
				Json::Value root(Json::objectValue);

				{
					Json::Value sensorRoot(Json::arrayValue);
					for (AllSensorProperties::iterator a = all.begin(); a != all.end(); a++) {
						for (TypeSensorProperties::iterator t = a->second.begin(); t != a->second.end(); t++) {
							for (SensorProperties::iterator n = t->second.begin(); n != t->second.end(); n++) {
								Json::Value property;
								property["type"] = a->first;
								property["name"] = t->first;
								property["property"] = n->first;
								property["value"] = n->second;

								sensorRoot.append(property);
							}
						}
					}
					root["sensor"] = sensorRoot;
				}

				{
					Json::Value globalRoot(Json::arrayValue);
					for (GlobalProperties::iterator a = global.begin(); a != global.end(); a++) {
						for (Properties::iterator n = a->second.begin(); n != a->second.end(); n++) {
							Json::Value property;
							property["name"] = a->first;
							property["property"] = n->first;
							property["value"] = n->second;

							globalRoot.append(property);
						}

					}
					root["global"] = globalRoot;
				}

				std::string obj = Json::StyledWriter().write(root);
				std::ofstream ostr(cfgName.c_str());
				ostr << obj;
			}

		private:
			Poco::AutoPtr<Poco::Util::XMLConfiguration> cfg;
	};

	const std::string Configuration::NameProperty = "Name";

	std::string Configuration::getSensorName(const std::string sensorType, const std::string & rawName) {
		try {
			return getSensorProperty(sensorType, rawName, NameProperty);
		} catch (...) {
			return rawName;
		}
		return rawName;
	}

	std::string Configuration::getSensorProperty(const std::string sensorType, const std::string & rawName,
			const std::string & propertyName) {
		ConfigurationImpl cimpl;
		return cimpl.getProperty(sensorType, rawName, propertyName);
	}

	Configuration::Property::List Configuration::getSensorProperties(const std::string sensorType, const std::string & rawName){
		ConfigurationImpl cimpl;
		return cimpl.getProperties(sensorType, rawName);
	}

	void Configuration::setSensorProperty(const std::string sensorType, const std::string & rawName,
			const std::string & propertyName, const std::string & value) {
		ConfigurationImpl cimpl;
		cimpl.setProperty(sensorType, rawName, propertyName, value);

	}

	std::string Configuration::getGlobalProperty(const std::string name, const std::string & propertyName) {
		ConfigurationImpl cimpl;
		return cimpl.getGlobalProperty(name, propertyName);
	}
	void Configuration::setGlobalProperty(const std::string name, const std::string & propertyName, const std::string & value) {
		ConfigurationImpl cimpl;
		cimpl.setGlobalProperty(name, propertyName, value);
	}

	std::string Configuration::initCfg(const std::string sectionName, const std::string & propertyName, const std::string & defaultValue){
		try {
			return BB::Configuration::getGlobalProperty(sectionName, propertyName);
		} catch (...){
			BB::Configuration::setGlobalProperty(sectionName, propertyName, defaultValue);
		}
		return defaultValue;
	}
	int Configuration::initCfg(const std::string sectionName, const std::string & propertyName, const int & defaultValue){
		std::string val = Poco::NumberFormatter::format(defaultValue);
		try {
			return Poco::NumberParser::parse(BB::Configuration::getGlobalProperty(sectionName, propertyName));
		} catch (...){
			BB::Configuration::setGlobalProperty(sectionName, propertyName, Poco::NumberFormatter::format(defaultValue));
		}
		return defaultValue;
	}

/*
 Configuration::Configuration() {
 Poco::File f("bb.xml");
 if (!f.exists()){
 std::ofstream os(f.path().c_str());
 os << "<bb>"
 "<server>"
 "<nicknames>"
 "</nicknames>"
 "</server>"
 "</bb>";
 }
 cfg = new Poco::Util::XMLConfiguration(f.path());
 }

 Configuration::~Configuration() {

 }

 Poco::AutoPtr<Poco::Util::ConfigurationView> Configuration::getView(std::string s){
 return new Poco::Util::ConfigurationView(s, cfg);
 }

 std::string Configuration::getString(Poco::AutoPtr<Poco::Util::ConfigurationView> & cfgView, std::string key, std::string def){
 if (cfgView.isNull()){
 return def;
 }
 if (!cfgView->has(key)){
 return def;
 }
 return cfgView->getString(key);
 }
 */
} /* namespace BB */
