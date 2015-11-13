/*
 * RemoteClientNode.h
 *
 *  Created on: Apr 10, 2015
 *      Author: dron
 */

#ifndef REMOTECLIENTNODE_H_
#define REMOTECLIENTNODE_H_
#include <BB/Node/BasicNode.h>
#include <BB/Node/Listener/FilteringNodeForwarder.h>
#include <BB/Node/Listener/NodeListener.h>
#include <BB/Node/RemoteClient/RemoteClientNodeForwarder.h>
#include "BB/Forwarder/RemoteClient/RemoteClient.h"
#include "BB/Node/INode.h"

namespace BB {

	class RemoteClientNode : public BasicNode {
		public:
			RemoteClientNode(std::string uid);
			virtual ~RemoteClientNode();

			AllData read();
		private:
			void onConnectionStatus(bool & );
			void onPendingQueue(int & );
			void onChanged(SettingsValues  & s);
			RemoteClientSettings retrieveSettings();
		private:
			RemoteClientNodeForwarder::Ptr rc;
			FilteringNodeForwarder::Ptr filter;
			NodeListener::Ptr listener;
			bool currentConnectionStatus;
			int pendingQueue;
	};

} /* namespace BB */

#endif /* REMOTECLIENTNODE_H_ */
