/*
 * ForecastParser.h
 *
 *  Created on: Feb 1, 2013
 *      Author: root
 */

#ifndef FORECASTPARSER_H_
#define FORECASTPARSER_H_

#include "Poco/SAX/ContentHandler.h"
#include "Poco/DateTime.h"
#include <Poco/BasicEvent.h>
#include <Poco/DateTimeFormatter.h>

namespace BB {

	namespace Forecast {

		struct ForecastData {
				std::string dateStr;
				Poco::DateTime date;
				int temperature;
				int wind;
				std::string info;
				int img;
				double precipitation;
		};

		class ForecastParser: public Poco::XML::ContentHandler {
			public:
				ForecastParser();
				virtual ~ForecastParser();

				void setDocumentLocator(const Poco::XML::Locator* loc);
				void startDocument();
				void endDocument();
				void startElement(const Poco::XML::XMLString& namespaceURI, const Poco::XML::XMLString& localName, const Poco::XML::XMLString& qname,
						const Poco::XML::Attributes& attributes);
				void endElement(const Poco::XML::XMLString& uri, const Poco::XML::XMLString& localName, const Poco::XML::XMLString& qname);

				void characters(const Poco::XML::XMLChar ch[], int start, int length);
				void ignorableWhitespace(const Poco::XML::XMLChar ch[], int start, int len);
				void processingInstruction(const Poco::XML::XMLString& target, const Poco::XML::XMLString& data);
				void startPrefixMapping(const Poco::XML::XMLString& prefix, const Poco::XML::XMLString& uri);
				void endPrefixMapping(const Poco::XML::XMLString& prefix);
				void skippedEntity(const Poco::XML::XMLString& name);

				Poco::BasicEvent<ForecastData> Forecast;
			private:
				ForecastData data;

		};

	}
} /* namespace BB */
#endif /* FORECASTPARSER_H_ */
