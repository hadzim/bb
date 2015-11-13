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

namespace BB {

		class GPOutSwitchNode: public SwitchNode {
			public:
				GPOutSwitchNode(std::string name, int number, bool defaultValue, int period);
			protected:
				virtual void performSwitch(bool on);
			private:
				exploringBB::GPIO gpout;
		};

} /* namespace BB */
#endif /* DALLASTEMPERATURESENSOR_H_ */
