/*
 * NodeTagFactCollector.cpp
 *
 *  Created on: Nov 10, 2015
 *      Author: dron
 */

#include <BB/Rules/Node/NodeTagFactCollector.h>

#include "TBS/Log.h"

namespace BB {

NodeTagFactCollector::NodeTagFactCollector(RuleEngine::Ptr engine) : engine(engine) {

}

NodeTagFactCollector::~NodeTagFactCollector() {

}

void NodeTagFactCollector::forward(const Node::Info & nodeInfo){

}
void NodeTagFactCollector::forward(const Node::Info & nodeInfo, const Node::Sensor & sensor, const Node::Data & nodeData){

	std::string uniqueID = nodeInfo.getUID()+sensor.name;

	Node::Data::Tags current = nodeData.tags();
	Node::Data::Tags last = lastNodeFacts[uniqueID];
	Node::Data::Tags newTags;
	Node::Data::Tags missingTags;

	std::set_difference(current.begin(), current.end(), last.begin(), last.end(),
		std::inserter(newTags, newTags.begin()));

	std::set_difference(last.begin(), last.end(), current.begin(), current.end(),
		std::inserter(missingTags, missingTags.begin()));

	lastNodeFacts[uniqueID] = current;

	for (auto t : newTags){
		BB::NodeTagFact::Data d= {
			nodeInfo,
			sensor,
			t
		};

		if (nodeInfo.getUID() == "temp@1"){
			LWARNING("update") << "add tag: " << t << " of " << nodeInfo.getUID() << LE;
		}
		engine->updateFact(new BB::NodeTagFact(d));
	}

	for (auto t : missingTags){
		BB::NodeTagFact::Data d = {
			nodeInfo,
			sensor,
			t
		};
		if (nodeInfo.getUID() == "temp@1"){
			LWARNING("update") << "remove tag: " << t << " of " << nodeInfo.getUID() << LE;
		}
		engine->removeFact(new BB::NodeTagFact(d));
	}
}


} /* namespace BB */
