/*
 * HttpServer.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#include "HttpServer.h"
#include <iostream>
#include <Poco/Delegate.h>
#include "Poco/Thread.h"

#include "BB/Services/Visualization.h"
#include "BB/Services/VisualizationSvc_Jsonp.h"
#include "BB/Services/Sensor.h"
#include "BB/Services/SensorSvc_DBus.h"
#include "BB/Sensor/SensorDataHelpers.h"

#include <iostream>

namespace BB {

std::string sensorKey(const std::string & type, const std::string & name) {
	return type + "::" + name;
}

std::string sensorKey(const TBS::BB::Visualization::SensorInfo & i) {
	return i.sensorType + "::" + i.sensorName;
}

std::string sensorKey(const BB::SensorData & d) {
	return d.getType() + "::" + d.getName();
}

std::string sensorDateKey(const BB::SensorData & d) {
	return sensorKey(d) + "::" + BB::SensorData::date2string(d.getDate());
}

class QueryProvider: public TBS::BB::Visualization::IQuery {
private:
	TBS::BB::Services::Sensor::DBus::Client::Ptr observer;
	Poco::Mutex m;
public:
	typedef Poco::SharedPtr<QueryProvider> Ptr;

	QueryProvider(){
		observer = new TBS::BB::Services::Sensor::DBus::Client();
		observer->DataDistributor().SensorDataReceived += Poco::delegate(this, &QueryProvider::onData);

	}
	~QueryProvider(){
		observer->DataDistributor().SensorDataReceived += Poco::delegate(this, &QueryProvider::onData);
	}

	void onData(TBS::BB::Services::Sensor::IDataDistributor::SensorDataReceivedArg & arg){

		Poco::Mutex::ScopedLock l(m);
		BB::SensorData sd = BB::SensorDataHelpers::eventArg2SensorData(arg);

		std::cout << "accept " << sd << std::endl;

		sensors[sensorKey(sd)].sensorName = sd.getName();
		sensors[sensorKey(sd)].sensorType = sd.getType();

		SensorDataMap & m = storage[sensorKey(sd)];

		m.erase(sensorDateKey(sd));

		m.insert(std::pair<std::string, BB::SensorData>(sensorDateKey(sd), sd));

		while (m.size() > 100){
			m.erase(m.begin());
		}

	}

	virtual std::vector<TBS::BB::Visualization::SensorInfo> GetSensors() {
		Poco::Mutex::ScopedLock l(m);
		std::vector<TBS::BB::Visualization::SensorInfo> all;

		for (Sensors::iterator i = sensors.begin(); i != sensors.end(); i++) {
			all.push_back(i->second);
		}

		return all;
	}

	static TBS::BB::Visualization::SensorData convertSD(const BB::SensorData & data){
			TBS::BB::Visualization::SensorData sd;
			sd.date = data.getDateAsString();
			sd.sensorName = data.getName();
			sd.sensorType = data.getType();
			sd.status = (int) data.getSensorStatus();
			sd.unit = data.getUnit();
			sd.value = data.getValue();
			sd.textValue = data.getTextValue();
			return sd;
		}

	virtual std::vector<TBS::BB::Visualization::SensorData> GetSensorData(
			const std::string & sensorType, const std::string & sensorName) {

		Poco::Mutex::ScopedLock l(m);
		std::vector<TBS::BB::Visualization::SensorData> all;

		SensorDataMap & m = storage[sensorKey(sensorType, sensorName)];
		for (SensorDataMap::iterator i = m.begin(); i != m.end(); i++) {

			all.push_back(convertSD(i->second));
		}
		return all;
	}



	virtual std::vector<TBS::BB::Visualization::SensorData> GetSensorsData(
				const std::string & sensorType) {

		Poco::Mutex::ScopedLock l(m);
		std::vector<TBS::BB::Visualization::SensorData> all;

		for (Storage::iterator sit = storage.begin(); sit != storage.end(); sit++){

			for (SensorDataMap::iterator i = sit->second.begin(); i != sit->second.end(); i++) {
				if (i->second.getType() == sensorType){
					all.push_back(convertSD(i->second));
				}
			}
		}
		return all;
	}

private:
	typedef std::map<std::string, BB::SensorData> SensorDataMap;
	typedef std::map<std::string, SensorDataMap> Storage;
	typedef std::map<std::string, TBS::BB::Visualization::SensorInfo> Sensors;
	Sensors sensors;
	Storage storage;
};

HttpServer::HttpServer() {

}

HttpServer::~HttpServer() {

}

int HttpServer::main(const std::vector<std::string>& args) {

	std::cout << "main HttpServer.start" << std::endl;

	TBS::Services::JsonServerChannel ch(8111);

	TBS::BB::Visualization::Json::Server::Ptr jsonServer =
			new TBS::BB::Visualization::Json::Server(ch);
	{

		QueryProvider::Ptr queryProvider = new QueryProvider();
		TBS::Services::IServer::Ptr sc = jsonServer->createQuery(queryProvider);
		jsonServer->start();
		this->waitForTerminationRequest();
		jsonServer->stop();
	}

	std::cout << "main HttpServer.stop" << std::endl;

	waitForTerminationRequest();

	return EXIT_OK;
}

} /* namespace BB */
