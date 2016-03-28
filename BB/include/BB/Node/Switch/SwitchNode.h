/*
 * SwitchNode.h
 *
 *  Created on: Feb 3, 2013
 *      Author: root
 */

#ifndef SwitchNode_H_
#define SwitchNode_H_
#include <BB/Node/BasicNode.h>

#include "TBS/Nullable.h"

namespace BB {

		class SwitchNode: public BasicNode {
			public:
				SwitchNode(std::string uid, bool defaultValue, BB::Node::Settings additinalSettings = BB::Node::Settings());
				~SwitchNode();

				AllData read();

			protected:

				virtual void performSwitch(bool on) = 0;
			protected:
				void onChanged(SettingsValues  & s);

				TBS::Nullable <bool> lastValue;
		};

} /* namespace BB */
#endif /* DALLASTEMPERATURESENSOR_H_ */
