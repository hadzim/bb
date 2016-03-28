#include <BB/Node/NodeTypes.h>
#include "BB/Node/Sensor/MotionNode.h"

#include <fstream>
#include <iostream>

namespace BB {


		static Node::Info motionInfo(std::string uid, BB::Node::Settings additionalSettings){

			BB::Node::Sensors sensors({ BB::Node::Sensor("Motion", BB::Node::Info::Motion, "%") });

			BB::Node::Settings settings({
				BB::Node::Setting("thresh", "range", BB::Node::Setting::Value(10)),
				BB::Node::Setting("security", "switch", BB::Node::Setting::Value(false)),
			});

			for (const auto & as : additionalSettings){
				settings.insert(as.second);
			}

			Node::Info info(uid, Node::Info::Motion, sensors, settings);
			return info;
		}


		MotionNode::MotionNode(std::string uid, int period, BB::Node::Settings additionalSettings) :
				SensorNode(motionInfo(uid, additionalSettings), period) {
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
			if (Node::isFilled(settings.at("security"), isSecurity) && isSecurity){
					data.tags().insert(tag + "[s]");
			}
		}



} /* namespace BB */
