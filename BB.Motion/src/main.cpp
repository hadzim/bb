/*
 * main.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#include <BB/Node/NodeFactory.h>
#include "BB/App2.h"
#include "BB/Node/INode.h"
#include "BB/Node/Board/BoardMotionNode.h"


#include "TBS/Log.h"


namespace BB {
	namespace Motion {


		class Factory: public DynamicNodeFactory {
			private:
				std::vector <INode::Ptr> motionNodes;

			public:

				Factory() : DynamicNodeFactory("Motion"){

					for (int i = 0; i < this->getCount(this->getName()); i++){
						std::stringstream s;
						s << "motion@" << (i+1);
						INode::Ptr n = new BoardMotionNode(s.str());
						motionNodes.push_back(n);
					}
					//pin P8_33 -> GPIO9
					//relayNode = new GPOutSwitchNode("relay@1", 9, false);
				}

				virtual int getCheckingPeriodInMs() {
					return 60000;
				}
				virtual INode::PtrList getNodes() {
					return motionNodes;
				}
		};

	}

} /* namespace BB */

NODE_BB_MAIN("Motion", BB::Motion::Factory)

