/*
 * OrCondition.h
 *
 *  Created on: Nov 11, 2015
 *      Author: dron
 */

#ifndef ORCONDITION_H_
#define ORCONDITION_H_
#include <BB/Rules/ICondition.h>
#include <BB/Rules/Serialization.h>

namespace BB {
class OrConditionRW;

class OrCondition : public ICondition {
public:
	typedef Poco::SharedPtr <ICondition> Ptr;

	OrCondition(ICondition::Ptr c1, ICondition::Ptr c2);
	OrCondition(std::vector <ICondition::Ptr> conditions);

	virtual bool isValid(const Facts & facts);

	NAMED_OBJECT("Or")

private:
	std::vector <ICondition::Ptr> conditions;
	friend class OrConditionRW;
};

class OrConditionRW : public TConditionRW <OrCondition> {
public:
	virtual Json::Value twrite(ConditionPtr & a);
	virtual ICondition::Ptr read(const Json::Value & value);

	NAMED_OBJECT(OrCondition::getClassName())
};

} /* namespace BB */

#endif /* ORCONDITION_H_ */
