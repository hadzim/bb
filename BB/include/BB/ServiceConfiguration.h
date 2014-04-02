/*
 * ServiceConfiguration.h
 *
 *  Created on: Feb 21, 2013
 *      Author: root
 */

#ifndef ServiceConfiguration_H_
#define ServiceConfiguration_H_

#include <Poco/Util/XMLConfiguration.h>
#include <Poco/Util/ConfigurationView.h>

namespace BB {

	class ServiceConfiguration {
		public:
			ServiceConfiguration();
			virtual ~ServiceConfiguration();

			Poco::AutoPtr<Poco::Util::ConfigurationView> getView(std::string s);
		protected:
			std::string getString(Poco::AutoPtr<Poco::Util::ConfigurationView> & cfgView, std::string key, std::string def);
		private:
			Poco::AutoPtr<Poco::Util::XMLConfiguration> cfg;

	};

} /* namespace BB */
#endif /* ServiceConfiguration_H_ */
