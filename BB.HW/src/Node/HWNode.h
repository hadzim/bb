/*
 * MotionNode.h
 *
 *  Created on: Feb 3, 2013
 *      Author: root
 */

#ifndef ScreenNodeMotionNode_H_
#define ScreenNodeMotionNode_H_
#include <BB/Node/BasicNode.h>

#include "UI/UI.h"


namespace BB {

		class HWNode: public BasicNode {
			public:
				HWNode(UI::Ptr ui);
				virtual ~HWNode();
				AllData read();
			protected:
				void onChanged(SettingsValues  & s);
			private:
				UI::Ptr ui;
		};

} /* namespace BB */
#endif /* DALLASTEMPERATURESENSOR_H_ */
