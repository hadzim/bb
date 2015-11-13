#include <BB/Node/NodeTypes.h>
#include "BB/Node/Status/SecurityNode.h"
#include <Poco/Delegate.h>
#include <fstream>
#include <iostream>

namespace BB {


		static Node::Info securityInfo(std::string uid){

			BB::Node::Sensors sensors({
				BB::Node::Sensor("Armed", BB::Node::Info::Status, ""),
				BB::Node::Sensor("Armable", BB::Node::Info::Status, ""),
				BB::Node::Sensor("Alarm", BB::Node::Info::Status, ""),
			});

			BB::Node::Settings settings({
				BB::Node::Setting("name"),
				BB::Node::Setting("place"),
				BB::Node::Setting("armed", "switch", BB::Node::Setting::Value(0)),
				BB::Node::Setting("pin", "text", BB::Node::Setting::Value("2468")),
			});

			Node::Info info(uid, Node::Info::Status, sensors, settings);
			return info;
		}
/*
		void SecurityNode::afterRead(Node::Data & data, const Node::Info & info, const Node::Sensor & sensor){
			auto settings = getSettings();
			std::string tag = "calm";

			double thresh = 0.0;
			if (Node::isFilled(settings.at("thresh"), thresh) && data.getValue() >= thresh){
				tag = "motion";
			}
			data.set("tag", tag);
		}
*/
		SecurityNode::SecurityNode(int period) :
				BasicNode(securityInfo("security"), period) {
			this->SettingsChanged += Poco::delegate(this, &SecurityNode::onChanged);
		}

		SecurityNode::~SecurityNode(){
			this->SettingsChanged -= Poco::delegate(this, &SecurityNode::onChanged);
		}

		SecurityNode::AllData SecurityNode::read(){
			std::cout << "security read" << std::endl;

			SecurityNode::AllData data;
			auto settings = this->getSettings();

			double val = 0.0;
			if (Node::isFilled(settings.at("armed"), val)){
				data.insert(std::make_pair("Armed", Node::Data(val, Node::localNow())));
			}

			//TODO - from TAGs
			data.insert(std::make_pair("Alarm", Node::Data(0, Node::localNow())));
			data.insert(std::make_pair("Armable", Node::Data(1, Node::localNow())));

			return data;
		}

		void SecurityNode::onChanged(SettingsValues  & s){
			std::cout << "Security data changed" << std::endl;
			bool reread = true;
			this->DataChanged.notify(this, reread);
		}


} /* namespace BB */
