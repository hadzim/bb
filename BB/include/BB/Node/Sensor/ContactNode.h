/*
 * ContactNode.h
 *
 *  Created on: Feb 3, 2013
 *      Author: root
 */

#ifndef ContactNode_H_
#define ContactNode_H_
#include "BB/Node/Sensor/SensorNode.h"

namespace BB {

		class ContactNode: public SensorNode {
			public:
				ContactNode(std::string uid, int period);
			protected:
				virtual void afterRead(Node::Data & data, const Node::Info & info, const Node::Sensor & sensor);
		};

} /* namespace BB */
#endif /* DALLASTEMPERATURESENSOR_H_ */
