#include <BB/Node/NodeTypes.h>
#include "BB/Node/Sensor/SensorNode.h"

#include <fstream>
#include <iostream>

namespace BB {

	SensorNode::SensorNode(Node::Info info, int period) :
			BasicNode(info, period) {

	}
	SensorNode::~SensorNode() {
	}

	SensorNode::AllData SensorNode::read() {
		std::cout << "sensor read" << std::endl;
		AllData data;
		Node::Info info = getInfo();
		for (const auto & sensor : info.getSensors()) {
			auto d = readOne(info, sensor.second);
			afterRead(d, info, sensor.second);
			data.insert(std::make_pair(sensor.first, d));
		}
		return data;
	}

	void SensorNode::afterRead(Node::Data & data, const Node::Info & info, const Node::Sensor & sensor) {

	}

} /* namespace BB */
