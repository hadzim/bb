/*
 * MotionNode.h
 *
 *  Created on: Feb 3, 2013
 *      Author: root
 */

#ifndef GPOutSwitchNode_H_
#define GPOutSwitchNode_H_
#include <BB/Node/Switch/SwitchNode.h>
#include <exploreBB/gpio/GPIO.h>
#include <Poco/SharedPtr.h>

namespace BB {

		class GPOutSwitchNode: public SwitchNode {
			public:
				GPOutSwitchNode(std::string name, int number, bool defaultValue);
				~GPOutSwitchNode();
			protected:
				virtual void performSwitch(bool on);
				void onChanged2(SettingsValues  & s);
			private:
				void check(SettingsValues  & s);
			private:
				Poco::SharedPtr <exploringBB::GPIO> gpout;

		};

} /* namespace BB */
#endif /* DALLASTEMPERATURESENSOR_H_ */
