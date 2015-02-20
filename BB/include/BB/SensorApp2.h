/*
 * SensorApp.h
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#ifndef SensorApp2_H_
#define SensorApp2_H_
#include <BB/Service/DataSender2.h>
#include "Poco/Util/ServerApplication.h"
#include "Poco/Timer.h"
#include "TBS/SimpleTimer.h"
#include "BB/Sensor/ISensor.h"
#include <vector>

#include "MQTT/MQTTClient.h"


namespace BB {

	class SensorApp2: public Poco::Util::ServerApplication {

		public:
			typedef Poco::SharedPtr<SensorApp2> Ptr;
			SensorApp2(std::string name, ISensorFactory2::Ptr factory);
			virtual ~SensorApp2();

		protected:

			int main(const std::vector<std::string>& args);
		private:
			void onTimer(TBS::SimpleTimer::TimerArg & t);
		private:
			std::string name;

			TBS::SimpleTimer::Ptr timer;

			ISensorFactory2::Ptr factory;
			IDataSender2::Ptr dataSender;

			//Poco::Timer dummy;
	};

} /* namespace BB */
#endif /* SensorApp_H_ */
