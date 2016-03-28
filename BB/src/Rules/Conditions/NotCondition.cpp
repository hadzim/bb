/*
 * NotCondition.cpp
 *
 *  Created on: Nov 11, 2015
 *      Author: dron
 */

#include <BB/Rules/Conditions/NotCondition.h>

namespace BB {

NotCondition::NotCondition(ICondition::Ptr c) : c(c) {

}

bool NotCondition::isValid(const Facts & facts){
	return !c->isValid(facts);
}

Json::Value NotConditionRW::twrite(ConditionPtr & a){
	Json::Value v(Json::objectValue);
	v["not"] = ConditionRW::write(a->c);
	return v;
}
ICondition::Ptr NotConditionRW::read(const Json::Value & value){
	return new NotCondition(ConditionRW::read(value["not"]));
}

REGISTER_CONDITIONRW(NotConditionRW::getClassName(), NotConditionRW, NotConditionRW_)

} /* namespace BB */
