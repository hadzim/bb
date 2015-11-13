#include <BB/Node/NodeTypes.h>
#include "BB/Node/Sensor/TemperatureNode.h"

#include <fstream>
#include <iostream>

namespace BB {


		static Node::Info tempInfo(std::string uid){

			BB::Node::Sensors sensors({ BB::Node::Sensor("Temperature", BB::Node::Info::Temperature, "C") });

			BB::Node::Settings settings({
				BB::Node::Setting("name"),
				BB::Node::Setting("place"),
				BB::Node::Setting("low"),
				BB::Node::Setting("high"),
				BB::Node::Setting("extremelyLow"),
				BB::Node::Setting("extremelyHigh"),
			});

			Node::Info info(uid, Node::Info::Temperature, sensors, settings);
			return info;
		}

		void TemperatureNode::afterRead(Node::Data & data, const Node::Info & info, const Node::Sensor & sensor){
			auto settings = getSettings();
			std::string tag = "normal";

			double thresh = 0.0;
			if (Node::isFilled(settings.at("extremelyLow"), thresh) && data.getValue() <= thresh){
				tag = "extremelyLow";
			} else if (Node::isFilled(settings.at("extremelyHigh"), thresh) && data.getValue() >= thresh){
				tag = "extremelyHigh";
			} else if (Node::isFilled(settings.at("low"), thresh) && data.getValue() <= thresh){
				tag = "low";
			} else if (Node::isFilled(settings.at("high"), thresh) && data.getValue() >= thresh){
				tag = "high";
			}

			data.tags().insert(tag);
			//data.set("tag", tag);
		}

		TemperatureNode::TemperatureNode(std::string uid, int period) :
				SensorNode(tempInfo(uid), period) {
		}


} /* namespace BB */
