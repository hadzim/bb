/*
 * NodeTagFact.h
 *
 *  Created on: Nov 7, 2015
 *      Author: dron
 */

#ifndef NODETAGFACT_H_
#define NODETAGFACT_H_
#include <BB/Node/NodeTypes.h>
#include <BB/Rules/IFact.h>
#include <Poco/SharedPtr.h>

namespace BB {

class NodeTagFact : public IFact {
public:
	typedef Poco::SharedPtr <NodeTagFact> Ptr;

	struct Data {
		Node::Info node;
		Node::Sensor sensor;
		std::string tag;
	};


	static bool isNodeTag(std::string topic);
	static Data parse(std::string topic, std::string value);

	NodeTagFact(Data d);
	virtual ~NodeTagFact();

	Node::Info node();
	Node::Sensor sensor();
	std::string tag();
private:

	Data data;
};

} /* namespace BB */

#endif /* NODETAGFACT_H_ */
