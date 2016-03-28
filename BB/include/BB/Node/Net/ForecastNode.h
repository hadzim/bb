/*
 * ForecastSensor.h
 *
 *  Created on: Feb 1, 2013
 *      Author: root
 */

#ifndef FORECASTNODE_H_
#define FORECASTNODE_H_

#include <BB/Node/BasicNode.h>

#include "TBS/Nullable.h"


namespace BB {
	namespace Forecast {
		struct ForecastData;

		class ForecastNode: public BasicNode {
			public:
				ForecastNode(std::string name);
				virtual ~ForecastNode();

				AllData read();
			private:
				void parse();
				void onData(ForecastData & data);
			private:
				std::string name;
				std::string url;
				std::vector <Node::Data> forecastData;
				TBS::Nullable <Node::Data> fToday;
				TBS::Nullable <Node::Data> fTonight;
				TBS::Nullable <Node::Data> fTomorrow;
				TBS::Nullable <Node::Data> fDATomorrow;

				std::string lastForecast;
				//Requests r;
		};

	}
} /* namespace BB */
#endif /* FORECAST_H_ */
