/*
 * NodeTagCondition.h
 *
 *  Created on: Nov 7, 2015
 *      Author: dron
 */

#ifndef NODETAGCONDITION_H_
#define NODETAGCONDITION_H_
#include <BB/Rules/ICondition.h>

#include "TBS/Nullable.h"

namespace BB {

class NodeTagCondition : public ICondition {
public:

	struct NodeMatch {
		TBS::Nullable<std::string> uid;
		TBS::Nullable<std::string> name;
		TBS::Nullable<std::string> place;
		TBS::Nullable<std::string> type;
	};

	struct SensorMatch {
		TBS::Nullable<std::string> name;
		TBS::Nullable<std::string> type;
	};

	typedef Poco::SharedPtr <NodeTagCondition> Ptr;

	NodeTagCondition(std::string tagValue, NodeMatch nodeMatch = NodeMatch(), SensorMatch sensorMatch = SensorMatch());
	virtual ~NodeTagCondition();

	virtual bool isValid(const Facts & facts);

private:
	std::string tagValue;
	NodeMatch node;
	SensorMatch sensor;
};

} /* namespace BB */

#endif /* NODETAGCONDITION_H_ */
