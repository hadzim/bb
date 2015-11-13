#include <BB/Node/NodeTypes.h>
#include "BB/Node/Sensor/MotionNode.h"

#include <fstream>
#include <iostream>

namespace BB {


		static Node::Info motionInfo(std::string uid){

			BB::Node::Sensors sensors({ BB::Node::Sensor("Motion", BB::Node::Info::Motion, "%") });

			BB::Node::Settings settings({
				BB::Node::Setting("name"),
				BB::Node::Setting("place"),
				BB::Node::Setting("thresh", "range", BB::Node::Setting::Value(10)),
				BB::Node::Setting("security", "switch", BB::Node::Setting::Value(false)),
			});

			Node::Info info(uid, Node::Info::Motion, sensors, settings);
			return info;
		}

		void MotionNode::afterRead(Node::Data & data, const Node::Info & info, const Node::Sensor & sensor){
			auto settings = getSettings();
			std::string tag = "calm";

			double thresh = 0.0;
			if (Node::isFilled(settings.at("thresh"), thresh) && data.getValue() >= thresh){
				tag = "motion";
			}
			data.tags().insert(tag);

			bool isSecurity = false;
			if (Node::isFilled(settings.at("security"), isSecurity) && isSecurity && data.getValue() >= thresh){
					data.tags().insert("insecure");
			}
		}

		MotionNode::MotionNode(std::string uid, int period) :
				SensorNode(motionInfo(uid), period) {
		}


} /* namespace BB */
