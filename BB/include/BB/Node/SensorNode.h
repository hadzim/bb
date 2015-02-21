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

#include "BB/Node/INode.h"

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
				for (const auto & sensor : info.getSensors()){
					auto d = readOne(info, sensor.second);
					data.insert(std::make_pair(sensor.first, d));
				}
				return data;
			}
		protected:
			virtual Node::Data readOne(const Node::Info & info, const Node::Sensor & sensor) = 0;
		private:
			Node::Info info;
			int period;
	};

} /* namespace TBS */
#endif /* ISENSOR_H_ */
