/*
 * NodeTagFact.cpp
 *
 *  Created on: Nov 7, 2015
 *      Author: dron
 */

#include <BB/Rules/Node/NodeTagFact.h>

namespace BB {


NodeTagFact::NodeTagFact(Data d) : IFact("NodeTag: " + d.node.getUID() + " sensor:" + d.sensor.name + " = " + d.tag), data(d) {

}

NodeTagFact::~NodeTagFact() {

}




Node::Info NodeTagFact::node(){
	return data.node;
}

Node::Sensor NodeTagFact::sensor(){
	return data.sensor;
}

std::string NodeTagFact::tag(){
	return data.tag;
}

} /* namespace BB */
