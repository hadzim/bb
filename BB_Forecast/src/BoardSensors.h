/*
 * BoardSensors.h
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#ifndef BoardSensors_H_
#define BoardSensors_H_
#include "Poco/Util/ServerApplication.h"
#include "Poco/Timer.h"
#include "TBS/SafeTimer.h"
#include "BB/Sensor/ISensor.h"

namespace BB {

	class BoardSensors: public Poco::Util::ServerApplication {

		public:
			typedef Poco::SharedPtr<BoardSensors> Ptr;
			BoardSensors();
			virtual ~BoardSensors();

		protected:

			int main(const std::vector<std::string>& args);
		private:
			void addSensor(ISensor::Ptr sensor);
			void onTimer(TBS::SafeTimer& t);
		private:
			static const int LED = 3;

			struct SensorData {
					typedef std::vector <SensorData> List;
					ISensor::Ptr sensor;
					TBS::SafeTimer::Ptr timer;
			};
			SensorData::List sensors;
			//Poco::Timer dummy;
	};

} /* namespace BB */
#endif /* BoardSensors_H_ */
