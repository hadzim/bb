/*
 * OrCondition.cpp
 *
 *  Created on: Nov 11, 2015
 *      Author: dron
 */

#include <BB/Rules/Conditions/OrCondition.h>

namespace BB {


OrCondition::OrCondition(ICondition::Ptr c1, ICondition::Ptr c2){
	conditions.push_back(c1);
	conditions.push_back(c2);
}
OrCondition::OrCondition(std::vector <ICondition::Ptr> conditions) : conditions(conditions){

}

bool OrCondition::isValid(const Facts & facts){
	for (auto c : conditions){
		if (c->isValid(facts)) return true;
	}
	return false;
}

Json::Value OrConditionRW::twrite(ConditionPtr & a){
	Json::Value v(Json::objectValue);
	v["or"] = Json::arrayValue;
	for (auto c : a->conditions){
		v["or"].append(ConditionRW::write(c));
	}
	return v;
}
ICondition::Ptr OrConditionRW::read(const Json::Value & value){
	std::vector <ICondition::Ptr> list;
	for (auto jv : value["or"]){
		list.push_back(ConditionRW::read(jv));
	}
	return new OrCondition(list);
}

REGISTER_CONDITIONRW(OrConditionRW::getClassName(), OrConditionRW, OrConditionRW_)


} /* namespace BB */
