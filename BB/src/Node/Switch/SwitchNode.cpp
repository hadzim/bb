#include <BB/Node/NodeTypes.h>
#include <BB/Node/Switch/SwitchNode.h>
#include <Poco/Delegate.h>

#include <fstream>
#include <iostream>

namespace BB {


		static Node::Info switchInfo(std::string uid, bool defaultValue){

			BB::Node::Sensors sensors({
					BB::Node::Sensor("Switch", BB::Node::Info::Switch, "")/*,
					BB::Node::Sensor("Switch-M", BB::Node::Info::Switch, ""),
					BB::Node::Sensor("Switch-A", BB::Node::Info::Switch, "")*/
			});

			BB::Node::Settings settings({
				BB::Node::Setting("valueManual", "switch", BB::Node::Setting::Value(defaultValue)),
				BB::Node::Setting("valueAuto", "switch", BB::Node::Setting::Value(defaultValue), {{"readonly", "readonly"}}),
				BB::Node::Setting("manualControl", "switch", BB::Node::Setting::Value(false)),
			});

			Node::Info info(uid, Node::Info::Switch, sensors, settings);
			return info;
		}

		SwitchNode::SwitchNode(std::string uid, bool defaultValue, int period) :
				BasicNode(switchInfo(uid, defaultValue), period) {
			this->SettingsChanged += Poco::delegate(this, &SwitchNode::onChanged);
		}

		SwitchNode::~SwitchNode(){
			this->SettingsChanged -= Poco::delegate(this, &SwitchNode::onChanged);
		}

		void SwitchNode::onChanged(SettingsValues  & s){
			std::cout << "Status data changed" << std::endl;
			bool reread = true;
			this->DataChanged.notify(this, reread);
		}

		SwitchNode::AllData SwitchNode::read(){
			std::cout << "switch read" << std::endl;

			SwitchNode::AllData data;
			auto settings = this->getSettings();

			auto cpy = lastValue;

			int autoVal = 0;
			if (Node::isFilled(settings.at("valueAuto"), autoVal)){
				//data.insert(std::make_pair("Switch-A", Node::Data(autoVal, Node::localNow())));
			}

			int manualVal = 0;
			if (Node::isFilled(settings.at("valueManual"), manualVal)){
				//data.insert(std::make_pair("Switch-M", Node::Data(manualVal, Node::localNow())));
			}

			int isManual = 0;
			if (Node::isFilled(settings.at("manualControl"), isManual)){
				Node::Data d(isManual ? manualVal : autoVal, Node::localNow());
				d.tags().insert(isManual ? "manual" : "auto");

				//d.tags().insert(std::string("manual:") + std::string(manualVal ? "1" : "0"));
				//d.tags().insert(std::string("auto:") + std::string(autoVal ? "1" : "0"));

				data.insert(std::make_pair("Switch", d));
				lastValue.set(d.getValue());
			}

			if (lastValue != cpy && lastValue.isSet()){
				this->performSwitch(lastValue.val());
			}

			return data;
		}


} /* namespace BB */
