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
			Configuration();
			virtual ~Configuration();

			Poco::AutoPtr<Poco::Util::ConfigurationView> getView(std::string s);
		protected:
			std::string getString(Poco::AutoPtr<Poco::Util::ConfigurationView> & cfgView, std::string key, std::string def);
		private:
			Poco::AutoPtr<Poco::Util::XMLConfiguration> cfg;

	};

} /* namespace BB */
#endif /* CONFIGURATION_H_ */
