/*
 * NodeTagCondition.h
 *
 *  Created on: Nov 7, 2015
 *      Author: dron
 */

#ifndef NODETAGCONDITION_H_
#define NODETAGCONDITION_H_
#include <BB/Rules/ICondition.h>
#include <BB/Rules/Serialization.h>

#include "TBS/Nullable.h"

namespace BB {

class NodeTagConditionRW;

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

	NAMED_OBJECT("NodeTag")

private:
	std::string tagValue;
	NodeMatch node;
	SensorMatch sensor;

	friend class NodeTagConditionRW;
};

class NodeTagConditionRW : public TConditionRW <NodeTagCondition> {
public:
	virtual Json::Value twrite(ConditionPtr & a);
	virtual ICondition::Ptr read(const Json::Value & value);

	NAMED_OBJECT(NodeTagCondition::getClassName())
};


} /* namespace BB */

#endif /* NODETAGCONDITION_H_ */
