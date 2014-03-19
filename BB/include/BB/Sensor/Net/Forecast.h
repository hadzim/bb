/*
 * Forecast.h
 *
 *  Created on: Feb 1, 2013
 *      Author: root
 */

#ifndef FORECAST_H_
#define FORECAST_H_

#include "BB/Sensor/ISensor.h"


namespace BB {

	struct ForecastData;

	class Forecast : public ISensor {
		public:
			Forecast(std::string name, std::string url);
			virtual ~Forecast();



			virtual Requests getRequests();
			virtual int getPeriodInMs();
			virtual std::string getName();
		private:
			void parse();
			void onData(ForecastData & data);
		private:
			std::string name;
			std::string url;
			Requests r;
	};

} /* namespace BB */
#endif /* FORECAST_H_ */
