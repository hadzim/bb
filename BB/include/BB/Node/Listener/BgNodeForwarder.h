/*
 * BgNodeForwarder.h
 *
 *  Created on: Apr 10, 2015
 *      Author: dron
 */

#ifndef BGNODEFORWARDER_H_
#define BGNODEFORWARDER_H_
#include <BB/Node/NodeTypes.h>
#include <Poco/Notification.h>

#include "INodeForwarder.h"
#include "TBS/NotificationBasedRunnable.h"

namespace BB {

	class NodeInfoNotification: public Poco::Notification {
		public:
			typedef Poco::AutoPtr<NodeInfoNotification> Ptr;
			NodeInfoNotification(const Node::Info & i);
			const Node::Info & nodeInfo() const;
		private:
			Node::Info i;
	};

	class NodeDataNotification: public Poco::Notification {
		public:
			typedef Poco::AutoPtr<NodeDataNotification> Ptr;
			NodeDataNotification(const Node::Info & i, const Node::Sensor & s, const Node::Data & d);

			const Node::Info & nodeInfo() const;
			const Node::Sensor & nodeSensor() const;
			const Node::Data & nodeData() const;

		private:
			Node::Info i;
			Node::Sensor s;
			Node::Data d;
	};

	class BgNodeForwarder: public INodeForwarder, public TBS::NotificationBasedRunnable {
		public:
			BgNodeForwarder(std::string name, INodeForwarder::Ptr wrappedForwarder);
			virtual ~BgNodeForwarder();

			virtual void forward(const Node::Info & nodeInfo);
			virtual void forward(const Node::Info & nodeInfo, const Node::Sensor & sensor, const Node::Data & nodeData);

		private:
			void processNotification(Poco::Notification::Ptr notification);
		private:
			INodeForwarder::Ptr wrappedForwarder;
	};

} /* namespace BB */

#endif /* BGNODEFORWARDER_H_ */
