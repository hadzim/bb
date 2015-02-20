/*
 * Configuration.h
 *
 *  Created on: Feb 21, 2013
 *      Author: root
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <Poco/Util/XMLConfiguration.h>
#include <Poco/Util/ConfigurationView.h>

namespace BB {

	class Configuration {
	public:
		struct Property {
			typedef std::vector <Property> List;
			std::string propName;
			std::string propValue;
		};
		static const std::string NameProperty;

		static void setSensorProperty(const std::string sensorType, const std::string & rawName, const std::string & propertyName, const std::string & value);

		static std::string getSensorProperty(const std::string sensorType, const std::string & rawName, const std::string & propertyName);

		static Property::List getSensorProperties(const std::string sensorType, const std::string & rawName);

		static std::string getSensorName(const std::string sensorType, const std::string & rawName);

		static std::string getGlobalProperty(const std::string sectionName, const std::string & propertyName);
		static void setGlobalProperty(const std::string sectionName, const std::string & propertyName, const std::string & value);

		static std::string initCfg(const std::string sectionName, const std::string & propertyName, const std::string & defaultValue);
		static int initCfg(const std::string sectionName, const std::string & propertyName, const int & defaultValue);


		/*
		public:
			Configuration();
			virtual ~Configuration();

			Poco::AutoPtr<Poco::Util::ConfigurationView> getView(std::string s);
		protected:
			std::string getString(Poco::AutoPtr<Poco::Util::ConfigurationView> & cfgView, std::string key, std::string def);
		private:
			Poco::AutoPtr<Poco::Util::XMLConfiguration> cfg;
		 */
	};



} /* namespace BB */
#endif /* CONFIGURATION_H_ */
