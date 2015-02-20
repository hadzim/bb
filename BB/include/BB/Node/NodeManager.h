/*
 * ISensor.h
 *
 *  Created on: Feb 1, 2013
 *      Author: root
 */

#ifndef INODE_H_
#define INODE_H_
#include <BB/Node/NodeTypes.h>
#include "Poco/SharedPtr.h"
#include <vector>
#include <string>

namespace BB {


	class NodeManager {
		public:
			NodeManager(INode::Ptr node){
				client = new TBS::MQTT::MQTTClient(node->getInfo().getUID());
				client->OnMessage += Poco::delegate(this, &NodeManager::onMessage);

				timer.Timer += Poco::delegate(this, &NodeManager::onTimer);

				const Node::Info & info = node->getInfo();

				//public node meta data
				//data channels
				for (const auto & key : info.getDataStreams().keys()){
					const Node::DataStream & dataStream = info.getDataStreams().get(key);

					std::string dataTopic = NodeChannel::dataTopic(info, dataStream);

					client->publish(dataTopic + "type", dataStream.type);
					client->publish(dataTopic + "unit", dataStream.unit);
				}

				//settings channels
				for (const auto & key : info.getSettings().keys()){
					const Node::Setting & s = info.getSettings().get(key);

					std::string topic = NodeChannel::settingTopic(info, s);

					client->publish(topic + "type",s.type);
					//TODO load value from somewhere
					client->publish(topic + "value",s.defaultValue.convert<std::string>());
					//subscribe settings changes
					client->subscribe(topic + "newvalue");
				}
			}

			~NodeManager(){
				timer.Timer -= Poco::delegate(this, &NodeManager::onTimer);

				client->OnMessage -= Poco::delegate(this, &NodeManager::onMessage);
				client = NULL;
			}
		private:


			void onTimer(TBS::SimpleTimer::TimerArg & arg){
				INode::AllData allData = node->read();
				const Node::Info & info = node->getInfo();
				for (auto & d : allData){
					const Node::DataStream & dataStream = info.getDataStreams().get(d.first);
					client->publish(
							NodeChannel::dataTopic(info, dataStream) + "current",
							RW::json2OneLine(NodeDataRW::write(d.second))
					);
				}
			}
			void onMessage(TBS::SimpleTimer::TimerArg & arg){
				//TODO react on new settings
			}
		private:
			TBS::MQTT::MQTTClient::Ptr client;
			INode::Ptr node;

			TBS::SimpleTimer timer;
	};

} /* namespace TBS */
#endif /* ISENSOR_H_ */
