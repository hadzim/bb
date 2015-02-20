/*
 * Task.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: dron
 */

#include <BB/Task/Task.h>
#include <sstream>
#include "Poco/Exception.h"
#include "BB/Sensor/SensorData.h"

#include "json/reader.h"
namespace BB {

	const std::string Task::UpdateStatus = "UpdateStatus";
	const std::string Task::CaptureCamera = "CaptureCamera";


	Task::Task(std::string what, std::string params, std::string source, std::string destination) :
			what(what), params(params), source(source), destination(destination) {

		Json::Reader r;
		r.parse(params, paramsJson);
	}

	std::string Task::getSource() const {
		return this->source;
	}
	std::string Task::getDestination() const {
		return this->destination;
	}

	std::string Task::getWhat() const {
		return this->what;
	}
	std::string Task::getParams() const {
		return this->params;
	}

	int Task::getIntParam(std::string paramName) const {
		return paramsJson[paramName].asInt();
	}
	std::string Task::getStringParam(std::string paramName) const {
		return paramsJson[paramName].asString();
	}


} /* namespace BB */

std::ostream & operator <<(std::ostream & o, const BB::Task & task) {
	o << "Task src:" << task.getSource() << " dest: " << task.getDestination() << " what:" << task.getWhat() << " params: " << task.getParams();
	return o;
}

