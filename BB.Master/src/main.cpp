/*
 * main.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#include "BB/App2.h"
#include "BB/Node/INode.h"
#include "BB/Node/Status/StatusNode.h"
#include "BB/Node/Status/SecurityNode.h"
#include "BB/Node/Status/HardwareNode.h"
#include "BB/Node/RemoteClient/RemoteClientNode.h"


namespace BB {
	namespace Master {
		class Factory: public INodeFactory {
			private:
				INode::Ptr rc;
			public:
				virtual int getCheckingPeriodInMs() {
					return 60000;
				}
				virtual INode::PtrList getNodes() {
					INode::PtrList nodes;

					{
						INode::Ptr sensorNode = new StatusNode(60 * 1000);
						nodes.push_back(sensorNode);
					}
					{
						INode::Ptr sensorNode = new SecurityNode(60 * 1000);
						nodes.push_back(sensorNode);
					}

					{
						if (!rc){
							rc = new RemoteClientNode("remote-local");
						}
						nodes.push_back(rc);
					}

					return nodes;
				}
		};

	}

} /* namespace BB */

NODE_BB_MAIN("Master", BB::Master::Factory)

