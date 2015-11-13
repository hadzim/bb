/*
 * BgNodeForwarder.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: dron
 */


#include <BB/Node/Listener/BgNodeForwarder.h>
#include "TBS/Log.h"

namespace BB {

	NodeInfoNotification::NodeInfoNotification(const Node::Info & i) :
			i(i) {

	}

	const Node::Info & NodeInfoNotification::nodeInfo() const {
		return this->i;
	}

	NodeDataNotification::NodeDataNotification(const Node::Info & i, const Node::Sensor & s, const Node::Data & d) :
			i(i), s(s), d(d) {

	}

	const Node::Data & NodeDataNotification::nodeData() const {
		return this->d;
	}
	const Node::Info & NodeDataNotification::nodeInfo() const {
		return this->i;
	}
	const Node::Sensor & NodeDataNotification::nodeSensor() const {
		return this->s;
	}

	BgNodeForwarder::BgNodeForwarder(std::string name, INodeForwarder::Ptr wrappedForwarder) :
			TBS::NotificationBasedRunnable(name), wrappedForwarder(wrappedForwarder) {

	}

	BgNodeForwarder::~BgNodeForwarder() {

	}

	void BgNodeForwarder::forward(const Node::Info & i, const Node::Sensor & s, const Node::Data & d) {
		//LDEBUG("RC") << "forward data " << d << LE;
		this->enqueueNotification(new NodeDataNotification(i, s, d));
	}

	void BgNodeForwarder::forward(const Node::Info & i) {
		//LDEBUG("RC") << "forward info " << i << LE;
		this->enqueueNotification(new NodeInfoNotification(i));
	}

	void BgNodeForwarder::processNotification(Poco::Notification::Ptr notification) {

		if (NodeDataNotification::Ptr msgNtf = notification.cast<NodeDataNotification>()) {
			Node::Data d = msgNtf->nodeData();
			Node::Info i = msgNtf->nodeInfo();
			Node::Sensor s = msgNtf->nodeSensor();
			wrappedForwarder->forward(i, s, d);
		}

		if (NodeInfoNotification::Ptr msgNtf = notification.cast<NodeInfoNotification>()) {
			Node::Info i = msgNtf->nodeInfo();
			wrappedForwarder->forward(i);
		}
	}

} /* namespace BB */
