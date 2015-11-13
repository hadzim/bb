#include <BB/Node/NodeTypes.h>
#include "BB/Node/Status/StatusNode.h"
#include <Poco/Delegate.h>
#include <fstream>
#include <iostream>

namespace BB {


		static Node::Info statusInfo(std::string uid){

			BB::Node::Sensors sensors({
				BB::Node::Sensor("Heating", BB::Node::Info::Status, "")
			});

			BB::Node::Settings settings({
				BB::Node::Setting("name"),
				BB::Node::Setting("place"),
				BB::Node::Setting("heating", "switch", BB::Node::Setting::Value(1)),
			});

			Node::Info info(uid, Node::Info::Status, sensors, settings);
			return info;
		}
/*
		void StatusNode::afterRead(Node::Data & data, const Node::Info & info, const Node::Sensor & sensor){
			auto settings = getSettings();
			std::string tag = "calm";

			double thresh = 0.0;
			if (Node::isFilled(settings.at("thresh"), thresh) && data.getValue() >= thresh){
				tag = "motion";
			}
			data.set("tag", tag);
		}
*/
		StatusNode::StatusNode(int period) :
				BasicNode(statusInfo("status"), period) {
			this->SettingsChanged += Poco::delegate(this, &StatusNode::onChanged);
		}

		StatusNode::~StatusNode(){
			this->SettingsChanged -= Poco::delegate(this, &StatusNode::onChanged);
		}

		StatusNode::AllData StatusNode::read(){
			std::cout << "status read" << std::endl;

			StatusNode::AllData data;
			auto settings = this->getSettings();

			double val = 0.0;
			if (Node::isFilled(settings.at("heating"), val)){
				data.insert(std::make_pair("Heating", Node::Data(val, Node::localNow())));
			}

			return data;
		}

		void StatusNode::onChanged(SettingsValues  & s){
			std::cout << "Status data changed" << std::endl;
			bool reread = true;
			this->DataChanged.notify(this, reread);
		}


} /* namespace BB */
