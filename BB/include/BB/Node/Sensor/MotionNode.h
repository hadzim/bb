/*
 * MotionNode.h
 *
 *  Created on: Feb 3, 2013
 *      Author: root
 */

#ifndef MotionNode_H_
#define MotionNode_H_
#include "BB/Node/Sensor/SensorNode.h"

namespace BB {

		class MotionNode: public SensorNode {
			public:
				MotionNode(std::string uid, int period);
			protected:
				virtual void afterRead(Node::Data & data, const Node::Info & info, const Node::Sensor & sensor);
		};

} /* namespace BB */
#endif /* DALLASTEMPERATURESENSOR_H_ */
