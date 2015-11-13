/*
 * INodeListener.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: dron
 */


#include <BB/Node/Listener/NodeListener.h>
#include "BB/DataRW.h"

#include "TBS/Log.h"

namespace BB {

	NodeListener::NodeListener(INodeForwarder::Ptr forwarder, std::string name) : forwarder(forwarder) {
		client = new TBS::MQTT::Client(name + "-listener");
		this->init();
	}

	NodeListener::NodeListener(INodeForwarder::Ptr forwarder, TBS::MQTT::Client::Ptr client) : forwarder(forwarder), client(client) {
		this->init();
	}

	void NodeListener::init(){
		client->OnMessage += Poco::delegate(this, &NodeListener::onMessage);
		client->subscribe("devices/+/nodes/+/data/+/meta/+");
		//client->subscribe("devices/+/nodes/+/settings/meta/+");
		client->subscribe("devices/+/nodes/+/data/+");
		client->subscribe("devices/+/nodes/+/settings/+");
		client->subscribe("devices/+/nodes/+/settings/+/meta/+");
	}

	NodeListener::~NodeListener(){
		client->OnMessage -= Poco::delegate(this, &NodeListener::onMessage);
	}
	void NodeListener::onMessage(TBS::MQTT::Message & msg){
		try {

			LTRACE("Listener") << "message on topic: " << msg.topic.topic() << " value: " << msg.topic.size() << LE;

		if (msg.topic.size() == 0){
			return;
		}
		if (msg.topic.part(0) != "devices"){
			return;
		}
		if (msg.topic.size() < 3){
			return;
		}
		if (msg.topic.part(2) != "nodes"){
			return;
		}
		if (msg.topic.size() < 4){
			return;
		}

		std::string nodeUID = msg.topic.part(3);

		LTRACE("Listener") << "nodeUID: " << nodeUID << LE;

/*
 * 		if (nodes.find(nodeUID) == nodes.end()){
			nodes.insert(std::make_pair(node, BB::Node::Info()));
		}
*/

		if (msg.topic.size() < 5){
			return;
		}
		//data processing
		if (msg.topic.part(4) == "data"){

			if (msg.topic.size() < 6){
				return;
			}
			std::string dataId = msg.topic.part(5);

			LTRACE("Listener") << "dataID: " << dataId << LE;

			if (msg.topic.size() == 6){

				LTRACE("Listener") << "data Value: " << msg.payload << LE;

				//data value
				std::string value = msg.payload;
				this->updateNodeData(nodeUID, dataId, value);
			} else {
				if (msg.topic.part(6) != "meta"){
					LTRACE("Listener") << "meta Value" << LE;
					return;
				}
				if (msg.topic.size() < 8){
					return;
				}

				LTRACE("Listener") << "info Value" << LE;

				std::string infoType = msg.topic.part(7);
				std::string value = msg.payload;
				this->updateNodeDataMeta(nodeUID, dataId, infoType, value);
			}

		}


		//data processing
		if (msg.topic.part(4) == "settings"){

			if (msg.topic.size() < 6){
				return;
			}
			std::string settingsId = msg.topic.part(5);

			LTRACE("Listener") << "settingsId: " << settingsId << LE;

			if (msg.topic.size() == 6){

				LTRACE("Listener") << "settings Value: " << msg.payload << LE;

				//data value
				std::string value = msg.payload;
				this->updateNodeSettings(nodeUID, settingsId, value);
			} else {
				if (msg.topic.part(6) != "meta"){
					LTRACE("Listener") << "meta Value" << LE;
					return;
				}
				if (msg.topic.size() < 8){
					return;
				}

				LTRACE("Listener") << "info Value" << LE;

				std::string infoType = msg.topic.part(7);
				std::string value = msg.payload;
				this->updateNodeSettingsMeta(nodeUID, settingsId, infoType, value);
			}

		}


		} catch (Poco::Exception & e){
			LERROR("Listener") << "error in message forwarding: " << e.displayText() << LE;
		} catch (std::exception & e){
			LERROR("Listener") << "error in message forwarding: " << e.what() << LE;
		}catch (...){
			LERROR("Listener") << "error in message forwarding: ???" << LE;
		}

	}

	void NodeListener::updateNodeDataMeta(std::string nodeId, std::string dataId, std::string info, std::string value){
		dataMeta[nodeId][dataId][info] = value;
		LWARNING("List") << "update info " << nodeId << " " << dataId << " " << info << " val " << value << LE;
		this->updateNodeInfo(nodeId);
	}

	void NodeListener::updateNodeInfo(std::string nodeId){
		Node::Sensors::List sensors;
		for (auto val : dataMeta[nodeId]){
			if (val.second.find("type") == val.second.end()){
				std::cout << "missing type" << std::endl;
				allNodes.erase(nodeId);
				return;
			}
			if (val.second.find("unit") == val.second.end()){
				std::cout << "missing unit" << std::endl;
				allNodes.erase(nodeId);
				return;
			}

			if (val.second["unit"] == "null"){
				val.second["unit"] = "";
			}
			Node::Sensor s(val.first, val.second["type"], val.second["unit"]);
			sensors.push_back(s);
		}

		Node::Setting::List settings;
		for (auto val : settingsMeta[nodeId]){

			if (val.second.find("type") == val.second.end()){
				std::cout << "missing settings type for " << nodeId << " first: " << val.first << std::endl;
				allNodes.erase(nodeId);
				return;
			}
			/*
			if (val.second.find("value") == val.second.end()){
				std::cout << "missing settings value for " << nodeId << " first: " << val.first << std::endl;
				allNodes.erase(nodeId);
				return;
			}*/
			Node::Setting s(val.first, val.second["type"]/*, val.second["value"]*/);
			settings.push_back(s);
		}


		//TODO type
		std::cout << "add node: " << nodeId << std::endl;

		Node::Info nodeInfo(nodeId, "", Node::Sensors(sensors), Node::Settings(settings));
		allNodes.erase(nodeId);
		allNodes.insert(std::make_pair(nodeId, nodeInfo));

		forwarder->forward(nodeInfo);

	}

	void NodeListener::updateNodeData(std::string nodeId, std::string dataId, std::string value){
		//checks to send data only for existing nodes
		if (allNodes.find(nodeId) == allNodes.end()){
			std::cout << "node info missing for " << nodeId << std::endl;
			return;
		}

		Node::Info info = allNodes.at(nodeId);

		if (!info.getSensors().has(dataId)){
			std::cout << "node data info missing" << std::endl;
			return;
		}

		Node::Sensor sensor = info.getSensors().at(dataId);

		LTRACE("Listener") << "parsing data " << value << LE;

		Node::Data data = NodeDataRW::read(RW::string2json(value));

		LNOTICE("Listener") << "new data for node: " << info << " data: " << data << LE;

		forwarder->forward(info, sensor, data);
	}


	void NodeListener::updateNodeSettingsMeta(std::string nodeId, std::string settingsId, std::string info, std::string value){
		settingsMeta[nodeId][settingsId][info] = value;
		updateNodeInfo(nodeId);
	}
	void NodeListener::updateNodeSettings(std::string nodeId, std::string settingsId, std::string value){
		settingsMeta[nodeId][settingsId]["value"] = value;
		updateNodeInfo(nodeId);
	}

} /* namespace BB */
