/*
 * FilteringNodeForwarder.h
 *
 *  Created on: 2.4.2014
 *      Author: JV
 */

#ifndef FilteringNodeForwarder_H_
#define FilteringNodeForwarder_H_

#include "TBS/Nullable.h"
#include "INodeForwarder.h"
#include "BB/Node/Listener/NodeFilter.h"

namespace BB {


	class FilteringNodeForwarder: public INodeForwarder {
		public:
			typedef Poco::SharedPtr <FilteringNodeForwarder> Ptr;

			FilteringNodeForwarder(INodeForwarder::Ptr wrappedForwarder);
			virtual ~FilteringNodeForwarder();

			void addFilter(INodeFilter::Ptr f);

			virtual void forward(const Node::Info & nodeInfo);
			virtual void forward(const Node::Info & nodeInfo, const Node::Sensor & nodeSensor, const Node::Data & nodeData);

		private:
			Poco::Mutex m;
			INodeForwarder::Ptr wrappedForwarder;
			typedef INodeFilter::PtrList Filters;
			Filters filters;
	};

} /* namespace BB */
#endif /* FilteringNodeForwarder_H_ */
