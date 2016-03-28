/*
 * SensorApp.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#include <BB/ServiceNotification.h>
#include "BB/SensorApp.h"
#include <iostream>
#include <Poco/Delegate.h>
#include "Poco/Thread.h"
//#include "BB/Sensor/Net/Forecast.h"
#include "BB/Sensor/SensorDataHelpers.h"
#include "BB/Services/DataSvc_DBus.h"

#include "TBS/Log.h"

namespace BB {

class SensorExecutor {
public:
	typedef Poco::SharedPtr<SensorExecutor> Ptr;
	typedef std::vector<Ptr> PtrList;

	SensorExecutor(ISensor::Ptr s);
	~SensorExecutor();

	Poco::BasicEvent<ISensor::Ptr> RequestsReady;
private:
	void onTimer(TBS::SimpleTimer::TimerArg & t);
private:
	TBS::SimpleTimer timer;
	ISensor::Ptr sensor;
};

SensorExecutor::SensorExecutor(ISensor::Ptr s) :
		sensor(s) {
	timer.Timer += Poco::delegate(this, &SensorExecutor::onTimer);
	timer.start(sensor->getPeriodInMs() / 20, sensor->getPeriodInMs());
}
SensorExecutor::~SensorExecutor() {
	timer.stop();
	timer.Timer -= Poco::delegate(this, &SensorExecutor::onTimer);
}

void SensorExecutor::onTimer(TBS::SimpleTimer::TimerArg & t) {
	this->RequestsReady(this, sensor);
}

SensorApp::SensorApp(std::string name, ISensorFactory::Ptr factory) :
		name(name), factory(factory) {

}

SensorApp::~SensorApp() {

}
/*
void SensorApp::addSensor(ISensor::Ptr sensor) {
	SensorData data;
	data.sensor = sensor;
	data.timer = new TBS::SimpleTimer();
	data.timer->Timer += Poco::delegate(this, &SensorApp::addSensor)
	sensors.push_back(data);
}*/

int SensorApp::main(const std::vector<std::string>& args) {

	Poco::Thread::sleep(1000);

	TBS::initLogs(name, 6, "/tmp/");

	this->client =  new TBS::BB::Services::Data::DBus::Client();

	std::cout << "main SensorApp.start" << std::endl;

	SensorExecutor::PtrList sensors;

	ISensorFactory::Sensors creating = factory->createSensors();
	for (ISensorFactory::Sensors::iterator i = creating.begin();
			i != creating.end(); i++) {
		SensorExecutor::Ptr s = new SensorExecutor(*i);
		sensors.push_back(s);
	}
	//events in
	for (SensorExecutor::PtrList::iterator s = sensors.begin();
			s != sensors.end(); s++) {
		(*s)->RequestsReady += Poco::delegate(this, &SensorApp::onRequest);
	}

	BB::ServiceNotification::serviceReady();

	this->waitForTerminationRequest();

	BB::ServiceNotification::serviceDisabled();

	//events out
	for (SensorExecutor::PtrList::iterator s = sensors.begin();
			s != sensors.end(); s++) {
		(*s)->RequestsReady -= Poco::delegate(this, &SensorApp::onRequest);
	}

	sensors.clear();

	std::cout << "main SensorApp.stop" << std::endl;

	return EXIT_OK;
}

void SensorApp::onRequest(ISensor::Ptr & t) {
	ISensor::Ptr sp = t;
	std::cout << "timer ticked for sensor " << sp->getName() << std::endl;

	ISensor::Requests reqs = sp->getRequests();
	for (ISensor::Requests::iterator r = reqs.begin(); r != reqs.end(); r++) {
		BB::SensorDataHelpers::sendData(client->DataCollector(), *r);
	}
}
} /* namespace BB */
