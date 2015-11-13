/*
 * NodeTagFactCollector.h
 *
 *  Created on: Nov 10, 2015
 *      Author: dron
 */

#ifndef NodeTagFactCollector_H_
#define NodeTagFactCollector_H_
#include <BB/Node/Listener/INodeForwarder.h>
#include <BB/Rules/Node/NodeTagFact.h>
#include <BB/Rules/RuleEngine.h>

namespace BB {

class NodeTagFactCollector : public INodeForwarder {
public:
	NodeTagFactCollector(RuleEngine::Ptr engine);
	virtual ~NodeTagFactCollector();
private:
	virtual void forward(const Node::Info & nodeInfo);
	virtual void forward(const Node::Info & nodeInfo, const Node::Sensor & sensor, const Node::Data & nodeData);
private:
	RuleEngine::Ptr engine;
	std::map <std::string, std::set <std::string> > lastNodeFacts;
};

} /* namespace BB */

#endif /* NodeTagFactCollector_H_ */
