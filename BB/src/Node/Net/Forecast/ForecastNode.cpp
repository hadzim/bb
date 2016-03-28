/*
 * Forecast.cpp
 *
 *  Created on: Feb 1, 2013
 *      Author: root
 */

#include "BB/Node/Net/ForecastNode.h"

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

#include "TBS/Log.h"

#include "BB/Node/INode.h"

#include "json/value.h"

#include "BB/DataRW.h"

namespace BB {
	namespace Forecast {

//		"Jizerka",
//		"http://www.yr.no/place/Czech_Republic/Liberec/Jizerka/"


		static Node::Info forecastInfo(std::string name){


			Node::Sensor today("A) Today", "Forecast", "C");
			Node::Sensor tonight("B) Tonight", "Forecast", "C");
			Node::Sensor tomorrow("C) Tomorrow", "Forecast", "C");
			Node::Sensor da("D) Day After", "Forecast", "C");

			Node::Setting place("place");
			Node::Setting url("url");


			Node::Info i(
					name,
					Node::Info::ForecastTemperature,
					Node::Sensors({today, tonight, tomorrow, da}),
					Node::Settings({place, url})
			);

			return i;
		}

		ForecastNode::ForecastNode(std::string name)
			: BasicNode(forecastInfo(name), /*1000 * 60 * 60 * 1*/ 1000*60*10) {

		}

		ForecastNode::~ForecastNode() {

		}


		ForecastNode::AllData ForecastNode::read(){
			LINFO("Forecast")<< "read" << LE;
			this->parse();

			ForecastNode::AllData all;
			if (fToday.isSet()){
				all.insert(std::make_pair("A) Today", fToday.val()));
			}
			if (fTonight.isSet()){
				all.insert(std::make_pair("B) Tonight", fTonight.val()));
			}
			if (fTomorrow.isSet()){
				all.insert(std::make_pair("C) Tomorrow", fTomorrow.val()));
			}
			if (fDATomorrow.isSet()){
				all.insert(std::make_pair("D) Day After", fDATomorrow.val()));
			}

			//publish future

			std::cout << "forecast future" << std::endl;

			Json::Value future(Json::arrayValue);
			for (auto d : forecastData){
				future.append(NodeDataRW::write(d));
			}

			std::string payload = RW::json2OneLine(future);

			std::cout << "forecast future: ..." << std::endl;

			if (lastForecast != payload){
				lastForecast = payload;
				auto sensors = this->getInfo().getSensors();
				for (const auto & s : sensors){
					AdditionalInfo ainfo;
					ainfo.topic = NodeChannel::dataTopic(getInfo(), s.second) + "/future";
					ainfo.payload = lastForecast;

					std::cout << "forecast future topic:  " << ainfo.topic << ": ..." << std::endl;

					this->AdditionalInfoRaised.notify(this, ainfo);

					std::cout << "forecast future topic:  " << ainfo.topic << ": done" << std::endl;

					break;
				}

			}

			return all;
		}

		static TBS::Nullable <Node::Data> findClosest(std::vector <Node::Data> data, Poco::DateTime destTime){
			TBS::Nullable <Node::Data> closest;
			int hourDiff = 5000;
			for (auto d : data){
				auto diff = d.getDate() - destTime;
				int currentHourDiff = std::abs(diff.totalHours() * 24);
				if (currentHourDiff < hourDiff){
					hourDiff = currentHourDiff;
					closest = d;
				}
			}
			return closest;
		}

		void ForecastNode::parse() {

			LINFO("Forecast")<< "parse" << LE;

			//r.clear();
			forecastData.clear();
			fToday.reset();
			fTonight.reset();
			fTomorrow.reset();
			fDATomorrow.reset();
			try {
				auto settings = getSettings();

				std::string forecastUrl = "";

				if (Node::isFilled(settings.at("url"), forecastUrl)){

					std::string url = forecastUrl + "forecast.xml";
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
					handler.Forecast += Poco::delegate(this, &ForecastNode::onData);

					Poco::XML::SAXParser parser;
					parser.setContentHandler(&handler);

					try {
						std::cout << "parse" << std::endl;
						parser.parse(&src);
					} catch (Poco::Exception& e) {
						std::cerr << e.displayText() << std::endl;
					}
					handler.Forecast -= Poco::delegate(this, &ForecastNode::onData);



				}

			} catch (Poco::Exception& exc) {
				std::cerr << exc.displayText() << std::endl;
			}

			Poco::DateTime now;
			Poco::DateTime mid(now.year(), now.month(), now.day(), 0, 0, 1);

			fToday = findClosest(forecastData, mid + Poco::Timespan(0, 12, 0, 0, 0));
			fTonight = findClosest(forecastData, mid + Poco::Timespan(1, 1, 0, 0, 0));
			fTomorrow = findClosest(forecastData, mid + Poco::Timespan(1, 12, 0, 0, 0));
			fDATomorrow = findClosest(forecastData, mid + Poco::Timespan(2, 12, 0, 0, 0));


		}

		void ForecastNode::onData(ForecastData & data) {

			std::stringstream str;
			str << "w:" << data.wind << ";";
			str << "i:" << data.img << ";";
			str << "p:" << data.precipitation << ";";
			str << "t:" << data.temperature << ";";
			str << "n:" << data.info << ";";

			Node::Data d(data.temperature, data.date);
			d.set("wind", Poco::NumberFormatter::format(data.wind));
			d.set("image", Poco::NumberFormatter::format(data.img));
			d.set("precipitation", Poco::NumberFormatter::format(data.precipitation));
			d.set("temperature", Poco::NumberFormatter::format(data.temperature));
			d.set("info", data.info);

			if (forecastData.size() < 13){
				forecastData.push_back(d);
			}

			LINFO("Forecast")<< "Read forecast: " << d << LE;
/*
			SensorData s(SensorData::ForecastTemperature, Configuration::getSensorName(SensorData::ForecastTemperature, name), name,
					SensorData::UnitTemperature, data.date, SensorData::Sensor_Ok, data.temperature, str.str());



			r.push_back(s);*/
		}



	}
} /* namespace BB */
