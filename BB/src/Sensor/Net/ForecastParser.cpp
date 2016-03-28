/*
 * ForecastParser.cpp
 *
 *  Created on: Feb 1, 2013
 *      Author: root
 */

#include "ForecastParser.h"
#include <iostream>
#include <Poco/DateTimeParser.h>
#include <Poco/SAX/Attributes.h>
#include "Poco/NumberParser.h"
#include <map>

static std::map <int, int> hourMap(){
	std::map <int, int> m;
	m[0] = m[1] = m[2] = m[3] = m[4] = 2;
	m[5] = m[6] = m[7] = m[8] = m[9] = m[10] = 8;
	m[11] = m[12] = m[13] = m[14] = m[15] = m[16] = 14;
	m[17] = m[18] = m[19] = m[20] = m[21] = m[22]  = m[23] = m[24] = 20;
	return m;
}

namespace BB {

	ForecastParser::ForecastParser() {

	}

	ForecastParser::~ForecastParser() {

	}

	void ForecastParser::setDocumentLocator(const Poco::XML::Locator* loc) {
	}
	void ForecastParser::startDocument() {
	}
	void ForecastParser::endDocument() {
	}

	void ForecastParser::startElement(const Poco::XML::XMLString& namespaceURI, const Poco::XML::XMLString& localName, const Poco::XML::XMLString& qname,
			const Poco::XML::Attributes& attributes) {

		static std::map <int, int> hmap = hourMap();

		if (localName == "time"){
			int tz = 0;
			data.dateStr =  attributes.getValue("", "from");
			std::cout << data.dateStr << std::endl;
			Poco::DateTime dt;
			Poco::DateTimeParser::parse(data.dateStr, dt, tz);
			data.date = Poco::DateTime(
					dt.year(),
					dt.month(),
					dt.day(),
					hmap[dt.hour()]
			);
		}
		if (localName == "temperature"){
			data.temperature = Poco::NumberParser::parse(attributes.getValue("", "value"));
		}
		if (localName == "symbol"){
			data.img = Poco::NumberParser::parse(attributes.getValue("", "number"));
			data.info = attributes.getValue("", "name");
		}
		if (localName == "windSpeed"){
			data.wind = Poco::NumberParser::parseFloat(attributes.getValue("", "mps"));
		}
		if (localName == "precipitation"){
			data.precipitation = Poco::NumberParser::parseFloat(attributes.getValue("", "value"));
		}
	}
	void ForecastParser::endElement(const Poco::XML::XMLString& uri, const Poco::XML::XMLString& localName, const Poco::XML::XMLString& qname) {
		//send event
		if (localName == "time"){
			std::cout << "notify " << data.dateStr << " " << data.temperature << std::endl;
			Forecast.notify(this, data);
		}
	}

	void ForecastParser::characters(const Poco::XML::XMLChar ch[], int start, int length) {
	}
	void ForecastParser::ignorableWhitespace(const Poco::XML::XMLChar ch[], int start, int len) {
	}
	void ForecastParser::processingInstruction(const Poco::XML::XMLString& target, const Poco::XML::XMLString& data) {
	}
	void ForecastParser::startPrefixMapping(const Poco::XML::XMLString& prefix, const Poco::XML::XMLString& uri) {
	}
	void ForecastParser::endPrefixMapping(const Poco::XML::XMLString& prefix) {
	}
	void ForecastParser::skippedEntity(const Poco::XML::XMLString& name) {
	}

} /* namespace BB */
