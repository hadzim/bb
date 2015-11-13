#include <BB/Node/NodeTypes.h>
#include "BB/Node/Switch/GPOutSwitchNode.h"

#include <fstream>
#include <iostream>

namespace BB {


		GPOutSwitchNode::GPOutSwitchNode(std::string name, int number, bool defaultValue, int period) :
				SwitchNode(name, defaultValue, period),
				gpout(number){
			gpout.setDirection(exploringBB::GPIO::OUTPUT);
			gpout.setActiveLow();

		}


		void GPOutSwitchNode::performSwitch(bool on){
			LERROR("SWITCHING") << "gpout to " << (on ? 1 : 0) << LE;
			gpout.setValue(on ? exploringBB::GPIO::HIGH : exploringBB::GPIO::LOW);
		}

} /* namespace BB */
