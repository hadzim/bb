/*
 * NodeTagCondition.cpp
 *
 *  Created on: Nov 7, 2015
 *      Author: dron
 */

#include <BB/Rules/Node/NodeTagCondition.h>
#include <BB/Rules/Node/NodeTagFact.h>

namespace BB {

NodeTagCondition::NodeTagCondition(std::string tagValue, NodeMatch nodeMatch, SensorMatch sensorMatch) : tagValue(tagValue), node(nodeMatch), sensor(sensorMatch) {

}

NodeTagCondition::~NodeTagCondition() {

}

static bool match(const std::string & tagValue, const NodeTagCondition::NodeMatch & nodeMatch, const NodeTagCondition::SensorMatch & sensorMatch, NodeTagFact::Ptr fact){
	if (tagValue != fact->tag()){
		return false;
	}

	auto factNode = fact->node();

	if (nodeMatch.uid.isSet() && nodeMatch.uid.cref() != factNode.getUID()){
		return false;
	}
	if (nodeMatch.type.isSet() && nodeMatch.type.cref() != factNode.getType()){
		return false;
	}
	if (nodeMatch.name.isSet() && nodeMatch.name.cref() != factNode.getSettings().at("name").defaultValue.convert<std::string>()){
		return false;
	}
	if (nodeMatch.place.isSet() && nodeMatch.place.cref() != factNode.getSettings().at("place").defaultValue.convert<std::string>()){
		return false;
	}

	auto sensorNode = fact->sensor();

	if (sensorMatch.name.isSet() && sensorMatch.name.cref() != sensorNode.name){
		return false;
	}
	if (sensorMatch.type.isSet() && sensorMatch.type.cref() != sensorNode.type){
		return false;
	}

	return true;
}

bool NodeTagCondition::isValid(const Facts & facts){
	for (auto f : facts){
		NodeTagFact::Ptr fact = f.second.cast<NodeTagFact>();
		if (!fact.isNull()){
			if (match(tagValue, node, sensor, fact)){
				return true;
			}
		}
	}
	return false;
}

} /* namespace BB */
