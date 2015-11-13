/*
 * DallasTemperatureSensor.h
 *
 *  Created on: Feb 3, 2013
 *      Author: root
 */

#ifndef TEMPERATURENODE_H_
#define TEMPERATURENODE_H_
#include "BB/Node/Sensor/SensorNode.h"

namespace BB {

		class TemperatureNode: public SensorNode {
			public:
				TemperatureNode(std::string uid, int period);
			protected:
				virtual void afterRead(Node::Data & data, const Node::Info & info, const Node::Sensor & sensor);
		};

} /* namespace BB */
#endif /* DALLASTEMPERATURESENSOR_H_ */
