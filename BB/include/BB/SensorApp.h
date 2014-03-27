/*
 * SensorApp.h
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#ifndef SensorApp_H_
#define SensorApp_H_
#include "Poco/Util/ServerApplication.h"
#include "Poco/Timer.h"
#include "TBS/SafeTimer.h"
#include "BB/Sensor/ISensor.h"
#include <vector>

namespace BB {

class ISensorFactory {
public:
	typedef std::vector<ISensor::Ptr> Sensors;
	typedef Poco::SharedPtr<ISensorFactory> Ptr;

	virtual ~ISensorFactory() {

	}
	virtual Sensors createSensors() = 0;
};

class SensorApp: public Poco::Util::ServerApplication {

public:
	typedef Poco::SharedPtr<SensorApp> Ptr;
	SensorApp(ISensorFactory::Ptr factory);
	virtual ~SensorApp();

protected:

	int main(const std::vector<std::string>& args);
private:
	void addSensor(ISensor::Ptr sensor);
	void onTimer(TBS::SafeTimer& t);
private:
	static const int LED = 3;

	struct SensorData {
		typedef std::vector<SensorData> List;
		ISensor::Ptr sensor;
		TBS::SafeTimer::Ptr timer;
	};
	SensorData::List sensors;
	ISensorFactory::Ptr factory;
	//Poco::Timer dummy;
};

} /* namespace BB */
#endif /* SensorApp_H_ */
