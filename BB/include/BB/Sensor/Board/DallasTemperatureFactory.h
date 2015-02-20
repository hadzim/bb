/*
 * DallasTemperatureSensor.h
 *
 *  Created on: Feb 3, 2013
 *      Author: root
 */

#ifndef DALLASTEMPERATURESENSOR2_H_
#define DALLASTEMPERATURESENSOR2_H_
#include "BB/Sensor/ISensor.h"


namespace BB {

	class DallasTemperatureFactory : public ISensorFactory2 {
		public:
			DallasTemperatureFactory();
			virtual ~DallasTemperatureFactory();

			virtual int getPeriodInMs();
			virtual ISensor2::PtrList getSensors();
	};

} /* namespace BB */
#endif /* DALLASTEMPERATURESENSOR_H_ */
