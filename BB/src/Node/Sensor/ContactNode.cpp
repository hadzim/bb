#include <BB/Node/NodeTypes.h>
#include "BB/Node/Sensor/ContactNode.h"

#include <fstream>
#include <iostream>

namespace BB {


		static Node::Info contactInfo(std::string uid){

			BB::Node::Sensors sensors({ BB::Node::Sensor("Contact", BB::Node::Info::Contact, "") });

			BB::Node::Settings settings({
				BB::Node::Setting("name"),
				BB::Node::Setting("place"),
				BB::Node::Setting("security", "switch", BB::Node::Setting::Value(false)),
			});

			Node::Info info(uid, Node::Info::Contact, sensors, settings);
			return info;
		}

		void ContactNode::afterRead(Node::Data & data, const Node::Info & info, const Node::Sensor & sensor){
			auto settings = getSettings();
			std::string tag = "opened";

			double thresh = 0.0;
			if (data.getValue() >= 1.0){
				tag = "closed";
			}
			data.tags().insert(tag);

			bool isSecurity = false;
			if (Node::isFilled(settings.at("security"), isSecurity) && isSecurity && data.getValue() == 0.0){
				data.tags().insert("insecure");
			}
		}

		ContactNode::ContactNode(std::string uid, int period) :
				SensorNode(contactInfo(uid), period) {
		}


} /* namespace BB */
