/*
 * main.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#include <BB/Node/NodeFactory.h>
#include "BB/App2.h"
#include "BB/Node/INode.h"
#include "BB/Node/Switch/GPOutSwitchNode.h"


#include "TBS/Log.h"


namespace BB {
	namespace Switch {


		class Factory: public DynamicNodeFactory {
			private:
				std::vector <INode::Ptr> switchNodes;

			public:

				Factory() : DynamicNodeFactory("Switch"){

					for (int i = 0; i < this->getCount(this->getName()); i++){
						std::stringstream s;
						s << "switch@" << (i+1);
						INode::Ptr n = new GPOutSwitchNode(s.str(), 0, false);
						switchNodes.push_back(n);
					}
				}

				virtual int getCheckingPeriodInMs() {
					return 60000;
				}
				virtual INode::PtrList getNodes() {
					return switchNodes;
				}
		};

	}

} /* namespace BB */

NODE_BB_MAIN("Switch", BB::Switch::Factory)


