/*
 * ISensor.h
 *
 *  Created on: Feb 1, 2013
 *      Author: root
 */

#ifndef ISENSOR_H_
#define ISENSOR_H_
#include "Poco/SharedPtr.h"
#include <vector>
#include <string>
#include "BB/Sensor/SensorData.h"

namespace BB {

	class ISensor {
		public:
			typedef Poco::SharedPtr <ISensor> Ptr;
			typedef SensorData::List Requests;
			virtual ~ISensor();

			virtual Requests getRequests() = 0;
			virtual int getPeriodInMs() = 0;
			virtual std::string getName() = 0;
	};

} /* namespace TBS */
#endif /* ISENSOR_H_ */
