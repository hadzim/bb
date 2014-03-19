/*
 * DallasTemperatureSensor.h
 *
 *  Created on: Feb 3, 2013
 *      Author: root
 */

#ifndef DALLASTEMPERATURESENSOR_H_
#define DALLASTEMPERATURESENSOR_H_
#include "BB/Sensor/ISensor.h"


namespace BB {

	class DallasTemperatureSensor : public ISensor {
		public:
			DallasTemperatureSensor();
			virtual ~DallasTemperatureSensor();

			virtual Requests getRequests();
			virtual int getPeriodInMs();
			virtual std::string getName();
	};

} /* namespace BB */
#endif /* DALLASTEMPERATURESENSOR_H_ */
