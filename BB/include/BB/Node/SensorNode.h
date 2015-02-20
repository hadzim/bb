/*
 * ISensor.h
 *
 *  Created on: Feb 1, 2013
 *      Author: root
 */

#ifndef SENSORNODE_H_
#define SENSORNODE_H_
#include "Poco/SharedPtr.h"
#include <vector>
#include <string>

namespace BB {


	class SensorNode : public INode {
		public:
			typedef Poco::SharedPtr <INode> Ptr;
			typedef std::vector<Ptr> PtrList;

			SensorNode(Node::Info info, int period) : info(info), period(period){

			}
			virtual ~SensorNode(){}

			virtual Node::Info getInfo(){
				return info;
			}
			virtual int getCheckingPeriodInMs(){
				return period;
			}

			virtual AllData read(){
				AllData data;
				for (const auto & sensor : info.getDataStreams().keys()){

				}
			}
		protected:
			void readOne() = 0;
		private:
			Node::Info info;
			int period;
	};

	class INodeFactory {
		public:

			typedef Poco::SharedPtr <INodeFactory> Ptr;

			virtual ~INodeFactory(){}

			virtual int getCheckingPeriodInMs() = 0;
			virtual INode::PtrList getNodes() = 0;
	};

} /* namespace TBS */
#endif /* ISENSOR_H_ */
