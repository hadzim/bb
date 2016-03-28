/*
 * NodeFactory.h
 *
 *  Created on: Dec 10, 2015
 *      Author: dron
 */

#ifndef NODEFACTORY_H_
#define NODEFACTORY_H_
#include <Poco/SharedPtr.h>

#include "BB/Node/INode.h"

namespace BB {

	class INodeFactory {
		public:

			typedef Poco::SharedPtr <INodeFactory> Ptr;

			virtual ~INodeFactory();

			virtual int getCheckingPeriodInMs() = 0;
			virtual INode::PtrList getNodes() = 0;
	};

	class DynamicNodeFactory : public INodeFactory {
		public:
			DynamicNodeFactory(std::string factoryName);
			virtual ~DynamicNodeFactory();

			std::string getName();

			static int getCount(std::string name);
			static void setCount(std::string name, int count);
		private:
			std::string name;
	};

} /* namespace BB */

#endif /* NODEFACTORY_H_ */
