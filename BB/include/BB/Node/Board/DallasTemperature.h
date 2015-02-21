/*
 * DallasTemperatureSensor.h
 *
 *  Created on: Feb 3, 2013
 *      Author: root
 */

#ifndef DALLASTEMPERATURENODE_H_
#define DALLASTEMPERATURENODE_H_
#include "BB/Node/SensorNode.h"

namespace BB {
	namespace DallasTemperature {

		class Helpers {
			public:

				static std::string dir();
				static std::string file(std::string uid);
		};

		class Node: public SensorNode {
			public:
				Node(BB::Node::Info info, int period);
				BB::Node::Data readOne(const BB::Node::Info & info, const BB::Node::Sensor & sensor);
		};

		class Factory: public INodeFactory {
			public:
				virtual int getCheckingPeriodInMs();
				virtual INode::PtrList getNodes();
		};

	}

} /* namespace BB */
#endif /* DALLASTEMPERATURESENSOR_H_ */
