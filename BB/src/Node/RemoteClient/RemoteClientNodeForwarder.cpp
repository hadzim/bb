/*
 * RemoteClientNodeForwarder.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: dron
 */
#include <BB/Node/RemoteClient/RemoteClientNodeForwarder.h>


namespace BB {

	RemoteClientNodeForwarder::RemoteClientNodeForwarder(const RemoteClientSettings & settings) : rc(settings) {

	}

	RemoteClientNodeForwarder::~RemoteClientNodeForwarder() {
	}

	void RemoteClientNodeForwarder::forward(const Node::Info & nodeInfo){
		rc.forward(nodeInfo);
	}

	void RemoteClientNodeForwarder::forward(const Node::Info & nodeInfo, const Node::Sensor & sensor, const Node::Data & nodeData){
		rc.forward(nodeInfo, sensor, nodeData);
	}

	RemoteClient & RemoteClientNodeForwarder::remoteClient(){
		return rc;
	}

} /* namespace BB */
