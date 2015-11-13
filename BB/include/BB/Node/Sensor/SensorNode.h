/*
 * ISensor.h
 *
 *  Created on: Feb 1, 2013
 *      Author: root
 */

#ifndef SENSORNODE_H_
#define SENSORNODE_H_
#include "Poco/SharedPtr.h"
#include <vector>
#include <string>

#include <BB/Node/BasicNode.h>

namespace BB {

	class SensorNode: public BasicNode {
		public:
			typedef Poco::SharedPtr<INode> Ptr;
			typedef std::vector<Ptr> PtrList;

			SensorNode(Node::Info info, int period);
			virtual ~SensorNode();

			virtual AllData read();

		protected:

			virtual void afterRead(Node::Data & data, const Node::Info & info, const Node::Sensor & sensor);

			virtual Node::Data readOne(const Node::Info & info, const Node::Sensor & sensor) = 0;

	};

} /* namespace TBS */
#endif /* ISENSOR_H_ */
