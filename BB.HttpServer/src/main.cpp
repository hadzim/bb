/*
 * main.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */



#include <BB/Configuration.h>
#include "BB/App.h"
#include "BB/ForwardApp.h"
#include "BB/Forwarder/HttpServer/HttpServerForwarder.h"

namespace BB {
	class Factory : public IForwarderFactory {
		virtual IForwarder::Ptr createForwarder(){

			int			port			= Configuration::initCfg("HttpServer", "port", 			8111);
			int			oneSensorLimit	= Configuration::initCfg("HttpServer", "oneSensorLimit",150);


			IFilter::PtrList filters;
			filters.push_back(
					new TimespanFilter(SensorData::Temperature, Poco::Timespan(0, 0, 10, 0, 0))
			);
			filters.push_back(
					new TimespanFilter(SensorData::Motion, Poco::Timespan(0, 0, 10, 0, 0))
			);
			filters.push_back(
					new EmptyFilter(SensorData::ForecastTemperature)
			);

			return new HttpServerForwarder(port, filters, oneSensorLimit);
		}
	};
}

FWD_BB_MAIN("HttpServer", BB::Factory)


