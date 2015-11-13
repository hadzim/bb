/*
 * main.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#include "BB/App2.h"
#include "BB/Node/INode.h"
#include "BB/Node/Switch/GPOutSwitchNode.h"


#include "TBS/Log.h"


namespace BB {
	namespace Relays {


		class Factory: public INodeFactory {
			private:
				INode::Ptr relayNode;

			public:

				Factory(){
					//pin P8_33 -> GPIO9
					relayNode = new GPOutSwitchNode("relay@1", 9, false, 60 * 1000);
				}

				virtual int getCheckingPeriodInMs() {
					return 60000;
				}
				virtual INode::PtrList getNodes() {
					INode::PtrList nodes;
					{
						nodes.push_back(relayNode);
					}
					return nodes;
				}
		};

	}

} /* namespace BB */

NODE_BB_MAIN("Relays", BB::Relays::Factory)

