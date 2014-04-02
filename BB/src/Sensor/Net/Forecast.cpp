/*
 * Forecast.cpp
 *
 *  Created on: Feb 1, 2013
 *      Author: root
 */

#include "BB/Sensor/Net/Forecast.h"

#include <Poco/SAX/SAXParser.h>
#include <Poco/SAX/InputSource.h>

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include <Poco/Net/HTTPCredentials.h>
#include "Poco/StreamCopier.h"
#include "Poco/NullStream.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include <iostream>
#include "ForecastParser.h"
#include <Poco/Delegate.h>

#include "ForecastParser.h"
#include <sstream>
#include "BB/Configuration.h"

namespace BB {

	Forecast::Forecast(std::string name, std::string url_) : name(name), url(url_ + "forecast.xml") {
		std::cout << "create url: " << url << std::endl;
	}

	Forecast::~Forecast() {

	}

	std::string Forecast::getName(){
		return name;
	}

	void Forecast::parse() {
		r.clear();
		try {
			std::cout << "get url: " << url << std::endl;

			//http://www.yr.no/place/Czech_Republic/Central_Bohemia/%C4%8Cerven%C3%A9_Pe%C4%8Dky/forecast_hour_by_hour.xml
			Poco::URI uri(url);

			std::string path(uri.getPathAndQuery());

			Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());
			Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, path, Poco::Net::HTTPMessage::HTTP_1_0);
			Poco::Net::HTTPResponse response;

			session.sendRequest(request);

			std::istream& rs = session.receiveResponse(response);

			std::cout << response.getStatus() << " " << response.getReason() << std::endl;
			if (response.getStatus() != Poco::Net::HTTPResponse::HTTP_OK) {
				throw Poco::Exception("Cannot get remote data");
			}

			//Poco::StreamCopier::copyStream(rs, std::cout);


			Poco::XML::InputSource src(rs);

			ForecastParser handler;
			handler.Forecast += Poco::delegate(this, &Forecast::onData);

			Poco::XML::SAXParser parser;
			parser.setContentHandler(&handler);

			try {
				std::cout << "parse" << std::endl;
				parser.parse(&src);
			} catch (Poco::Exception& e) {
				std::cerr << e.displayText() << std::endl;
			}
			handler.Forecast -= Poco::delegate(this, &Forecast::onData);

		} catch (Poco::Exception& exc) {
			std::cerr << exc.displayText() << std::endl;
		}

	}

	void Forecast::onData(ForecastData & data){

		std::stringstream str;
		str << "w:" << data.wind << ";";
		str << "i:" << data.img << ";";
		str << "p:" << data.precipitation << ";";
		str << "t:" << data.temperature << ";";
		str << "n:" << data.info << ";";

		SensorData s(
				SensorData::ForecastTemperature,
				Configuration::getSensorName(SensorData::ForecastTemperature, name),
				name,
				SensorData::UnitTemperature,
				data.date,
				SensorData::Sensor_Ok,
				data.temperature,
				str.str()
		);
		r.push_back(s);
	}

	Forecast::Requests Forecast::getRequests(){
		this->parse();
		return r;
	}
	int Forecast::getPeriodInMs(){
		//each 4 hours
		return 1000*60*10;//1000*60*60*2; //60*4;
	}



} /* namespace BB */
