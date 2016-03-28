/*
 * AndCondition.h
 *
 *  Created on: Nov 11, 2015
 *      Author: dron
 */

#ifndef ANDCONDITION_H_
#define ANDCONDITION_H_
#include <BB/Rules/ICondition.h>
#include <BB/Rules/Serialization.h>
#include <vector>

namespace BB {

class AndConditionRW;

class AndCondition : public ICondition {
public:
	typedef Poco::SharedPtr <ICondition> Ptr;

	AndCondition(ICondition::Ptr c1, ICondition::Ptr c2);
	AndCondition(std::vector <ICondition::Ptr> conditions);

	virtual bool isValid(const Facts & facts);

	NAMED_OBJECT("And")
private:
	std::vector <ICondition::Ptr> conditions;
	friend class AndConditionRW;
};

class AndConditionRW : public TConditionRW <AndCondition> {
public:
	virtual Json::Value twrite(ConditionPtr & a);
	virtual ICondition::Ptr read(const Json::Value & value);

	NAMED_OBJECT(AndCondition::getClassName())
};

} /* namespace BB */

#endif /* ANDCONDITION_H_ */
