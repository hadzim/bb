/*
 * Configuration.cpp
 *
 *  Created on: Feb 21, 2013
 *      Author: root
 */

#include "BB/Configuration.h"
#include <fstream>
#include "Poco/File.h"
namespace BB {

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

} /* namespace BB */
