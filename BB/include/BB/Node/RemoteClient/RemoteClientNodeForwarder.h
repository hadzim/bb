/*
 * RemoteClientNodeForwarder.h
 *
 *  Created on: Apr 10, 2015
 *      Author: dron
 */

#ifndef REMOTECLIENTNODEFORWARDER_H_
#define REMOTECLIENTNODEFORWARDER_H_
#include <Poco/SharedPtr.h>
#include "BB/Forwarder/RemoteClient/RemoteClient.h"
#include "../Listener/INodeForwarder.h"
namespace BB {

	class RemoteClientNodeForwarder : public INodeForwarder {
		public:

			typedef Poco::SharedPtr <RemoteClientNodeForwarder> Ptr;

			RemoteClientNodeForwarder(const RemoteClientSettings & settings);
			virtual ~RemoteClientNodeForwarder();

			virtual void forward(const Node::Info & nodeInfo);
			virtual void forward(const Node::Info & nodeInfo, const Node::Sensor & sensor, const Node::Data & nodeData);

			RemoteClient & remoteClient();
		private:
			RemoteClient rc;
	};

} /* namespace BB */

#endif /* REMOTECLIENTNODEFORWARDER_H_ */
