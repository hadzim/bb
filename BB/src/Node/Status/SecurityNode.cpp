#include <BB/Node/NodeTypes.h>
#include "BB/Node/Status/SecurityNode.h"
#include <Poco/Delegate.h>
#include <fstream>
#include <iostream>

namespace BB {


		static Node::Info securityInfo(std::string uid){

			BB::Node::Sensors sensors({
				//BB::Node::Sensor("Armed", BB::Node::Info::Status, ""),
				//BB::Node::Sensor("Armable", BB::Node::Info::Status, ""),
				BB::Node::Sensor("Alarm", BB::Node::Info::Status, ""),
			});

			BB::Node::Settings settings({

				BB::Node::Setting("armed", "switch", BB::Node::Setting::Value(0)),
				BB::Node::Setting("pin", "text", BB::Node::Setting::Value("2468")),

				BB::Node::Setting("alarmManual", "switch", BB::Node::Setting::Value(false)),
				BB::Node::Setting("alarmAuto", "switch", BB::Node::Setting::Value(false), {{"readonly", "readonly"}}),
				BB::Node::Setting("manualAlarmControl", "switch", BB::Node::Setting::Value(false)),
			});

			Node::Info info(uid, Node::Info::Status, sensors, settings);
			return info;
		}

		SecurityNode::SecurityNode(int period) :
				BasicNode(securityInfo("security"), period) {
			this->SettingsChanged += Poco::delegate(this, &SecurityNode::onChanged);
		}

		SecurityNode::~SecurityNode(){
			this->SettingsChanged -= Poco::delegate(this, &SecurityNode::onChanged);
		}

		SecurityNode::AllData SecurityNode::read(){
			std::cout << "switch read" << std::endl;

			SecurityNode::AllData data;
			auto settings = this->getSettings();

			int autoVal = 0;
			if (Node::isFilled(settings.at("alarmAuto"), autoVal)){

			}

			int manualVal = 0;
			if (Node::isFilled(settings.at("alarmManual"), manualVal)){

			}

			int armedVal = 0;
			if (Node::isFilled(settings.at("armed"), armedVal)){

			}

			int isManual = 0;
			if (Node::isFilled(settings.at("manualAlarmControl"), isManual)){
				Node::Data d(isManual ? manualVal : autoVal, Node::localNow());
				d.tags().insert(isManual ? "manual" : "auto");

				if (d.getValue() > 0){
					d.tags().insert("alarm");
				}

				if (armedVal > 0){
					d.tags().insert("armed");
				}

				data.insert(std::make_pair("Alarm", d));
			}

			return data;

		}

		void SecurityNode::onChanged(SettingsValues  & s){
			std::cout << "Security data changed" << std::endl;
			bool reread = true;
			this->DataChanged.notify(this, reread);
		}


} /* namespace BB */
