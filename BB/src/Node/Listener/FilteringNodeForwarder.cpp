/*
 * FilteringNodeForwarder.cpp
 *
 *  Created on: 2.4.2014
 *      Author: JV
 */


#include <BB/Node/Listener/FilteringNodeForwarder.h>
#include "TBS/Log.h"

namespace BB {

FilteringNodeForwarder::FilteringNodeForwarder(INodeForwarder::Ptr wrappedForwarder) : wrappedForwarder(wrappedForwarder) {

}

FilteringNodeForwarder::~FilteringNodeForwarder() {

}

void FilteringNodeForwarder::addFilter(INodeFilter::Ptr f){
	Poco::Mutex::ScopedLock l(m);
	this->filters.push_back(f);
}

void FilteringNodeForwarder::forward(const Node::Info & nodeInfo, const Node::Sensor & nodeSensor, const Node::Data & nodeData){
	TBS::Nullable <Node::Data> result;
	{
		Poco::Mutex::ScopedLock l(m);
		for (Filters::iterator i = filters.begin(); i != filters.end(); i++){
			TBS::Nullable <Node::Data> res = (*i)->filter(nodeInfo, nodeSensor, nodeData);
			if (res.isSet()){
				std::cout << "Pass filter info: " << nodeInfo << " sensor: " << nodeSensor << " data: " << nodeData << std::endl;
				result = res;
				break;
			}
		}
	}
	if (result.isSet()){
		//std::cout << "filter data OK" << std::endl;
		wrappedForwarder->forward(nodeInfo, nodeSensor, result.ref());
	} else {
		//std::cout << "filter data REJECTED" << std::endl;
	}
}

void FilteringNodeForwarder::forward(const Node::Info & nodeInfo){
	LINFO("RC") << "forward status" << LE;
	wrappedForwarder->forward(nodeInfo);
}

} /* namespace BB */
