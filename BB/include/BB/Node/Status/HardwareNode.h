/*
 * MotionNode.h
 *
 *  Created on: Feb 3, 2013
 *      Author: root
 */

#ifndef HWStatusNodeMotionNode_H_
#define HWStatusNodeMotionNode_H_
#include <BB/Node/BasicNode.h>


namespace BB {

		class HardwareNode: public BasicNode {
			public:
				HardwareNode(int period);

				AllData read();
			protected:

		};

} /* namespace BB */
#endif /* DALLASTEMPERATURESENSOR_H_ */
