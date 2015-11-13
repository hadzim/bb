/*
 * MotionNode.h
 *
 *  Created on: Feb 3, 2013
 *      Author: root
 */

#ifndef StatusNodeMotionNode_H_
#define StatusNodeMotionNode_H_
#include <BB/Node/BasicNode.h>


namespace BB {

		class StatusNode: public BasicNode {
			public:
				StatusNode(int period);
				virtual ~StatusNode();
				AllData read();


			protected:
				void onChanged(SettingsValues  & s);
		};

} /* namespace BB */
#endif /* DALLASTEMPERATURESENSOR_H_ */
