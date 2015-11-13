/*
 * INodeForwarder.h
 *
 *  Created on: Apr 10, 2015
 *      Author: dron
 */

#ifndef INODEFORWARDER_H_
#define INODEFORWARDER_H_
#include <BB/Node/NodeTypes.h>
#include <Poco/SharedPtr.h>

namespace BB {

	class INodeForwarder {
		public:
			typedef Poco::SharedPtr <INodeForwarder> Ptr;

			virtual ~INodeForwarder();

			virtual void forward(const Node::Info & nodeInfo) = 0;
			virtual void forward(const Node::Info & nodeInfo, const Node::Sensor & sensor, const Node::Data & nodeData) = 0;
	};

} /* namespace BB */

#endif /* INODEFORWARDER_H_ */
