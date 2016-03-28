/*
 * AndCondition.cpp
 *
 *  Created on: Nov 11, 2015
 *      Author: dron
 */

#include <BB/Rules/Conditions/AndCondition.h>

namespace BB {

AndCondition::AndCondition(ICondition::Ptr c1, ICondition::Ptr c2){
	conditions.push_back(c1);
	conditions.push_back(c2);
}
AndCondition::AndCondition(std::vector <ICondition::Ptr> conditions) : conditions(conditions){

}

bool AndCondition::isValid(const Facts & facts){
	for (auto c : conditions){
		if (!c->isValid(facts)) return false;
	}
	return true;
}

Json::Value AndConditionRW::twrite(ConditionPtr & a){
	Json::Value v(Json::objectValue);
	v["and"] = Json::arrayValue;
	for (auto c : a->conditions){
		v["and"].append(ConditionRW::write(c));
	}
	return v;
}
ICondition::Ptr AndConditionRW::read(const Json::Value & value){
	std::vector <ICondition::Ptr> list;
	for (auto jv : value["and"]){
		list.push_back(ConditionRW::read(jv));
	}
	return new AndCondition(list);
}

REGISTER_CONDITIONRW(AndConditionRW::getClassName(), AndConditionRW, AndConditionRW_)

} /* namespace BB */
