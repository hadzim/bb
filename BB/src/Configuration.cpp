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

namespace BB {

	typedef std::map <std::string, std::string> SensorProperties;
	typedef std::map <std::string, SensorProperties> TypeSensorProperties;
	typedef std::map <std::string, TypeSensorProperties> AllSensorProperties;

	const std::string cfgName = "configuration.json";

	class ConfigurationImpl {
	private:
		AllSensorProperties all;
	public:
		ConfigurationImpl(){
			load();
		}

		std::string getProperty(std::string type, std::string name, std::string property) {
			return all.at(type).at(name).at(property);
		}
		void setProperty(std::string type, std::string name, std::string property, std::string value){
			all[type][name][property] = value;
		}

	private:
		void load(){
			try {
				Json::Value root;
				{
					std::ifstream istr(cfgName.c_str());
					Json::Reader r;
					r.parse(istr, root);
				}
				if (root.isArray()){
					const Json::Value & root2 = root;
					for (Json::Value::const_iterator a = root2.begin(); a != root2.end(); a++){
						try {
							const Json::Value & property = *a;
							all
								[property["type"].asString()]
								[property["name"].asString()]
								[property["property"].asString()] = property["value"].asString();
						} catch (...){

						}
					}
				}
			} catch (...){

			}
		}
		void save(){
			Json::Value root(Json::arrayValue);
			for (AllSensorProperties::iterator a = all.begin(); a != all.end(); a++){
				for (TypeSensorProperties::iterator t = a->second.begin(); t != a->second.end(); t++){
					for (SensorProperties::iterator n = t->second.begin(); n != t->second.end(); n++){
						Json::Value property;
						property["type"] = a->first;
						property["name"] = t->first;
						property["property"] = n->first;
						property["value"] = n->second;
						root.append(property);
					}
				}
			}

			std::string obj = Json::FastWriter().write(root);
			std::ofstream ostr(cfgName.c_str());
			ostr << obj;
		}

	private:
		Poco::AutoPtr<Poco::Util::XMLConfiguration> cfg;
	};

	const std::string Configuration::NameProperty = "Name";

	std::string Configuration::getSensorName(const std::string sensorType, const std::string & rawName){
		try {
			return getSensorProperty(sensorType, rawName, NameProperty);
		} catch (...){
			return rawName;
		}
		return rawName;
	}

	std::string Configuration::getSensorProperty(const std::string sensorType, const std::string & rawName, const std::string & propertyName){
		ConfigurationImpl cimpl;
		return cimpl.getProperty(sensorType, rawName, propertyName);
	}

	void Configuration::setSensorProperty(const std::string sensorType, const std::string & rawName, const std::string & propertyName, const std::string & value){
		ConfigurationImpl cimpl;
		cimpl.setProperty(sensorType, rawName, propertyName, value);
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
