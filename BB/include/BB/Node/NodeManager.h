/*
 * ISensor.h
 *
 *  Created on: Feb 1, 2013
 *      Author: root
 */

#ifndef NODEMANAGER_H_
#define NODEMANAGER_H_
#include <BB/Node/NodeTypes.h>
#include "Poco/SharedPtr.h"
#include <vector>
#include <string>

#include "BB/DataRW.h"

#include "MQTT/MQTTClient.h"

namespace BB {


	class NodeManager {
		public:

			typedef Poco::SharedPtr <NodeManager> Ptr;

			NodeManager(INode::Ptr node){
				client = new TBS::MQTT::MQTTClient(node->getInfo().getUID());
				client->OnMessage += Poco::delegate(this, &NodeManager::onMessage);

				timer.Timer += Poco::delegate(this, &NodeManager::onTimer);

				const Node::Info & info = node->getInfo();

				//public node meta data
				//data channels
				for (const auto & sensor : info.getSensors()){
					const Node::Sensor & s = sensor.second;

					std::string dataTopic = NodeChannel::dataTopic(info, s);

					client->publish(dataTopic + "type", s.type);
					client->publish(dataTopic + "unit", s.unit);
				}

				//settings channels
				for (const auto & setting : info.getSettings()){
					const Node::Setting & s = setting.second;

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
					const Node::Sensor & sensor = info.getSensors().at(d.first);
					client->publish(
							NodeChannel::dataTopic(info, sensor) + "current",
							RW::json2OneLine(NodeDataRW::write(d.second))
					);
				}
			}
			void onMessage(TBS::MQTT::Message & arg){
				//TODO react on new settings
			}
		private:
			TBS::MQTT::MQTTClient::Ptr client;
			INode::Ptr node;

			TBS::SimpleTimer timer;
	};

} /* namespace TBS */
#endif /* ISENSOR_H_ */
