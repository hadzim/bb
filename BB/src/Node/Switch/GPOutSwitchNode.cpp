#include <BB/Node/NodeTypes.h>
#include <Poco/Delegate.h>
#include "BB/Node/Switch/GPOutSwitchNode.h"

#include <fstream>
#include <iostream>

namespace BB {

		static BB::Node::Settings gpoutSwitchAdditional(int number){

			BB::Node::Settings s({
				BB::Node::Setting("gpoutNumber", "text", BB::Node::Setting::Value(number)),
			});

			return s;
		}


		GPOutSwitchNode::GPOutSwitchNode(std::string name, int number, bool defaultValue) :
				SwitchNode(name, defaultValue, gpoutSwitchAdditional(number)){
			this->SettingsChanged += Poco::delegate(this, &GPOutSwitchNode::onChanged2);
		}
		GPOutSwitchNode::~GPOutSwitchNode(){
			this->SettingsChanged -= Poco::delegate(this, &GPOutSwitchNode::onChanged2);
		}

		void GPOutSwitchNode::check(SettingsValues  & s){
			int lastNumber = 0;
			if (gpout){
				lastNumber = gpout->getNumber();
			}

			int pinNumber = 0;

			Node::isFilled(s.at("gpoutNumber"), pinNumber);

			if (pinNumber && pinNumber != lastNumber){
				gpout = NULL;
				LERROR("GPOut") << "AfterRead: " << pinNumber << LE;
				gpout = new exploringBB::GPIO(pinNumber);
				gpout->setDirection(exploringBB::GPIO::OUTPUT);
				gpout->setActiveLow();
				LERROR("GPOut") << "AfterRead done: " << pinNumber << LE;
			}
		}

		void GPOutSwitchNode::onChanged2(SettingsValues  & s){
			check(s);
		}

		void GPOutSwitchNode::performSwitch(bool on){
			auto s = getSettings();
			check(s);

			LERROR("SWITCHING") << "Perform gpout to " << (on ? 1 : 0) << LE;
			if (gpout){
				LERROR("SWITCHING") << "Perform gpout " << gpout->getNumber() << " to " << (on ? 1 : 0) << LE;
				gpout->setValue(on ? exploringBB::GPIO::HIGH : exploringBB::GPIO::LOW);
			}
		}

} /* namespace BB */
