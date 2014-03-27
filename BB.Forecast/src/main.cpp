/*
 * main.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */



#include "BB/App.h"
#include "BB/SensorApp.h"
#include "BB/Sensor/Net/Forecast.h"

namespace BB {
	class ForecastFactory : public ISensorFactory {
		virtual Sensors createSensors(){
			Sensors s;
			s.push_back(
					new Forecast(
							"JosefuvDul",
							"http://www.yr.no/place/Czech_Republic/Liberec/Josef%C5%AFv_D%C5%AFl/"
					)
			);

			s.push_back(
					new Forecast(
							"Smedava",
							"http://www.yr.no/place/Czech_Republic/Liberec/Sm%C4%9Bdavsk%C3%A1_hora/"
					)
			);

			s.push_back(
					new Forecast(
							"Kristianov",
							"http://www.yr.no/place/Czech_Republic/Liberec/Kristi%C3%A1nov~3072707/"
					)
			);


			return s;
		}
	};
}


SENSOR_BB_MAIN(BB::ForecastFactory)

