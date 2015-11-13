/*
 * INodeListener.h
 *
 *  Created on: Apr 10, 2015
 *      Author: dron
 */

#ifndef INODELISTENER_H_
#define INODELISTENER_H_
#include <Poco/SharedPtr.h>
#include <map>

#include "TBS/MQTT/Client.h"

#include "INodeForwarder.h"

namespace BB {

	class NodeListener {
		public:
			typedef Poco::SharedPtr <NodeListener> Ptr;

			NodeListener(INodeForwarder::Ptr forwarder, std::string name);
			NodeListener(INodeForwarder::Ptr forwarder, TBS::MQTT::Client::Ptr client);
			~NodeListener();
		private:
			void init();
			void onMessage(TBS::MQTT::Message & msg);

			void updateNodeDataMeta(std::string nodeId, std::string dataId, std::string info, std::string value);
			void updateNodeData(std::string nodeId, std::string dataId, std::string value);

			void updateNodeSettingsMeta(std::string nodeId, std::string settingsId, std::string info, std::string value);
			void updateNodeSettings(std::string nodeId, std::string settingsId, std::string value);
			void updateNodeInfo(std::string nodeId);
		private:
			INodeForwarder::Ptr forwarder;
			TBS::MQTT::Client::Ptr client;


			typedef std::map <std::string, std::string> Meta;
			typedef std::map <std::string, Meta> NodeMeta;
			typedef std::map <std::string, NodeMeta> NodesMeta;
			//typedef std::map <std::string, bool> NodesComplete;
			typedef std::map <std::string, Node::Info> NodesInfo;
			NodesMeta dataMeta;
			NodesMeta settingsMeta;
			NodesInfo allNodes;


	};

} /* namespace BB */

#endif /* INODELISTENER_H_ */
