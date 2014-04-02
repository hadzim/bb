/*
 * Filter.h
 *
 *  Created on: 2.4.2014
 *      Author: JV
 */

#ifndef FILTER_H_
#define FILTER_H_
#include <Poco/SharedPtr.h>
#include <Poco/DateTime.h>
#include <Poco/Timespan.h>
#include "TBS/Nullable.h"
#include "BB/Sensor/SensorData.h"

namespace BB {

	class IFilter {
		public:
			typedef Poco::SharedPtr<IFilter> Ptr;
			virtual ~IFilter() {
			}
			virtual TBS::Nullable<SensorData> filter(const SensorData & data) = 0;
	};

	class SingleTimespanFilter : public IFilter {
		public:
			typedef Poco::SharedPtr<IFilter> Ptr;
			SingleTimespanFilter(std::string sensorKey, Poco::Timespan timespan);

			TBS::Nullable<SensorData> filter(const SensorData & data);
		private:
			std::string sensorKey;
			Poco::Timespan timespan;
			Poco::DateTime nextAcceptable;
	};

	class TimespanFilter : public IFilter {
		public:
			TimespanFilter(std::string sensorType, Poco::Timespan timespan);

			TBS::Nullable<SensorData> filter(const SensorData & data);
		private:
			std::string sensorType;
			Poco::Timespan timespan;

			std::map <std::string, SingleTimespanFilter> filters;

	};

} /* namespace BB */
#endif /* FILTER_H_ */
