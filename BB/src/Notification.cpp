/*
 * RuntimeStatus.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: dron
 */

#include <BB/Notification.h>
#include <sstream>
#include "Poco/Exception.h"
#include "BB/Sensor/SensorData.h"

namespace BB {

	Notification::NamesLevel::NamesLevel() {
		this->readableEnum[Info] = "Info";
		this->readableEnum[Warning] = "Warning";
		this->readableEnum[Alert] = "Alert";
	}

	Notification::NamesLevel Notification::namesLevel;

	Notification::Notification(Level level, std::string source, std::string message) : level(level), source(source), message(message), date(SensorData::localNow()) {
	}
	Notification::Notification(Level level, std::string source, std::string message, Poco::DateTime date): level(level), source(source), message(message), date(date) {

	}

	Notification::Level Notification::getLevel() const {
		return this->level;
	}
	std::string Notification::getSource() const {
		return this->source;
	}
	std::string Notification::getMessage() const {
		return this->message;
	}
	Poco::DateTime Notification::getDate() const {
		return this->date;
	}
	std::string Notification::getDateAsString() const {
		return SensorData::date2string(date);
	}

	std::string Notification::readableLevel(Level level) {
		return namesLevel.toString(level);
	}

} /* namespace BB */

std::ostream & operator <<(std::ostream & o, const BB::Notification & ntf) {
	o <<  "(" << BB::Notification::readableLevel(ntf.getLevel()) << ", " << ntf.getSource() << ", " << ntf.getMessage() << ")";
	return o;
}

