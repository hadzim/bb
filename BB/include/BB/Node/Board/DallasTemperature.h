/*
 * DallasTemperatureSensor.h
 *
 *  Created on: Feb 3, 2013
 *      Author: root
 */

#ifndef DALLASTEMPERATURENODE_H_
#define DALLASTEMPERATURENODE_H_
#include "BB/Node/Sensor/TemperatureNode.h"

namespace BB {
	namespace DallasTemperature {

		class Helpers {
			public:

				static std::string dir();
				static std::string file(std::string uid);
		};

		class Node: public TemperatureNode {
			public:
				Node(std::string uid, int period);
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
