/*
 * main.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#include <BB/Node/NodeFactory.h>
#include "BB/App2.h"
#include "BB/Node/INode.h"
#include "BB/Node/Sensor/CameraNode.h"


#include "TBS/Log.h"

namespace BB {
	namespace Cams {


		class Factory: public DynamicNodeFactory {
			private:
				std::vector <INode::Ptr> cameraNodes;

			public:

				Factory() : DynamicNodeFactory("Camera"){

					for (int i = 0; i < this->getCount(this->getName()); i++){
						std::stringstream s;
						s << "camera@" << (i+1);
						INode::Ptr n = new CameraNode(s.str());
						cameraNodes.push_back(n);
					}
				}

				virtual int getCheckingPeriodInMs() {
					return 60000;
				}
				virtual INode::PtrList getNodes() {
					return cameraNodes;
				}
		};

	}

} /* namespace BB */

NODE_BB_MAIN("Cameras", BB::Cams::Factory)

