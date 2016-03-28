/*
 * NotCondition.h
 *
 *  Created on: Nov 11, 2015
 *      Author: dron
 */

#ifndef NOTCONDITION_H_
#define NOTCONDITION_H_
#include <BB/Rules/ICondition.h>
#include <BB/Rules/Serialization.h>

namespace BB {

class NotConditionRW;

class NotCondition : public ICondition {
public:
	NotCondition(ICondition::Ptr c);
	virtual bool isValid(const Facts & facts);

	NAMED_OBJECT("Not")
private:
	friend class NotConditionRW;
	ICondition::Ptr c;
};

class NotConditionRW : public TConditionRW <NotCondition> {
public:
	virtual Json::Value twrite(ConditionPtr & a);
	virtual ICondition::Ptr read(const Json::Value & value);

	NAMED_OBJECT(NotCondition::getClassName())
};

} /* namespace BB */

#endif /* NOTCONDITION_H_ */
