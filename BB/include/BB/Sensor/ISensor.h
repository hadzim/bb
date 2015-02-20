/*
 * ISensor.h
 *
 *  Created on: Feb 1, 2013
 *      Author: root
 */

#ifndef ISENSOR_H_
#define ISENSOR_H_
#include <BB/Sensor/SensorData2.h>
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

	class ISensor2 {
		public:
			typedef Poco::SharedPtr <ISensor2> Ptr;
			typedef std::vector<Ptr> PtrList;

			virtual ~ISensor2();

			virtual Sensor::Info getInfo() = 0;
			virtual Sensor::Data getData() = 0;
	};

	//for logic in factory
	class Sensor2 : public ISensor2 {
		public:
			Sensor2(Sensor::Info i, Sensor::Data d);
			virtual Sensor::Info getInfo();
			virtual Sensor::Data getData();
		private:
			Sensor::Info info;
			Sensor::Data data;
	};

	class ISensorFactory2 {
		public:
			typedef Poco::SharedPtr <ISensorFactory2> Ptr;

			virtual ~ISensorFactory2(){}

			virtual int getPeriodInMs() = 0;
			virtual ISensor2::PtrList getSensors() = 0;
	};

	//for logic in sensor
	class SensorFactory2 : public ISensorFactory2 {
		public:
			SensorFactory2(int timeoutMs);
			virtual ISensor2::PtrList getSensors();
			void addSensor(ISensor2::Ptr s);
		private:
			int timeoutMs;
			ISensor2::PtrList sensors;
	};

} /* namespace TBS */
#endif /* ISENSOR_H_ */
