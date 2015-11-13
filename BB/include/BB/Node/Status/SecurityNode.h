/*
 * MotionNode.h
 *
 *  Created on: Feb 3, 2013
 *      Author: root
 */

#ifndef SecurityNodeMotionNode_H_
#define SecurityNodeMotionNode_H_
#include <BB/Node/BasicNode.h>


namespace BB {

		class SecurityNode: public BasicNode {
			public:
				SecurityNode(int period);
				virtual ~SecurityNode();
				AllData read();


			protected:
				void onChanged(SettingsValues  & s);
		};

} /* namespace BB */
#endif /* DALLASTEMPERATURESENSOR_H_ */
