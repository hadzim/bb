/*
 * Filter.cpp
 *
 *  Created on: 2.4.2014
 *      Author: JV
 */

#include "BB/Forwarder/Filter.h"
#include "BB/Sensor/SensorDataHelpers.h"

namespace BB {

	SingleTimespanFilter::SingleTimespanFilter(std::string sensorKey, Poco::Timespan timespan) :
			sensorKey(sensorKey), timespan(timespan) {
		//nextAcceptable = nextAcceptable + timespan;
	}

	TBS::Nullable<SensorData> SingleTimespanFilter::filter(const SensorData & data) {
		TBS::Nullable<SensorData> filtered;
		if (SensorDataHelpers::sensorID(data) != this->sensorKey) {
			return filtered;
		}

		//std::cout << "TimeCmp: " << SensorData::date2string(this->nextAcceptable) << " vs " << SensorData::date2string(data.getDate()) << std::endl;

		if (data.getDate() < this->nextAcceptable) {
			//std::cout << "single: bad date" << std::endl;
			return filtered;
		}

		filtered.set(data);

		//std::cout << "Next accept before: " << SensorData::date2string(this->nextAcceptable) << std::endl;

		do {
			this->nextAcceptable = this->nextAcceptable + this->timespan;
		} while (data.getDate() >= this->nextAcceptable);

		//std::cout << "Next accept after : " << SensorData::date2string(this->nextAcceptable) << std::endl;

		return filtered;
	}

	TimespanFilter::TimespanFilter(std::string sensorType, Poco::Timespan timespan) :
			sensorType(sensorType), timespan(timespan) {

	}

	TBS::Nullable<SensorData> TimespanFilter::filter(const SensorData & data) {

		TBS::Nullable<SensorData> filtered;

		if (data.getType() != this->sensorType) {
			//std::cout << "multi: wrong type " << data.getType() << " vs " << this->sensorType << std::endl;
			return filtered;
		}

		std::string key = SensorDataHelpers::sensorID(data);
		if (filters.find(key) == filters.end()) {
			filters.insert(std::make_pair(key, SingleTimespanFilter(key, timespan)));
		}

		return filters.at(key).filter(data);
	}

} /* namespace BB */
