/*
 * Serialization.cpp
 *
 *  Created on: Nov 16, 2015
 *      Author: dron
 */

#include "BB/Rules/Serialization.h"


namespace BB {

Json::Value RulesRW::write(const Rules & rules) {
	Json::Value json(Json::arrayValue);
	for (auto r : rules) {
		json.append(RuleRW::write(r.second));
	}
	return json;
}

Rules RulesRW::read(const Json::Value & value) {
	Rules rules;
	for (auto r : value) {
		Rule rule = RuleRW::read(r);
		rules.insert(std::make_pair(rule.name(), rule));
	}
	return rules;
}

Json::Value RuleRW::write(const Rule & r) {
	Rule cpy = r;
	Json::Value v(Json::objectValue);
	v["name"] = r.name();
	v["action"] = ActionRW::write(cpy.action());
	v["negativeAction"] = ActionRW::write(cpy.negativeAction());
	v["condition"] = ConditionRW::write(cpy.condition());
	return v;
}
Rule RuleRW::read(const Json::Value & value) {
	std::string name = value["name"].asString();
	IAction::Ptr a = ActionRW::read(value["action"]);
	IAction::Ptr an = ActionRW::read(value["negativeAction"]);
	ICondition::Ptr c = ConditionRW::read(value["condition"]);
	return Rule(name, c, a, an);
}

Json::Value StringRW::write(const std::string & v){
	return Json::Value(v);
}
std::string StringRW::read(const Json::Value & value){
	return value.asString();
}

Json::Value ActionRW::write(const IAction::Ptr & v) {
	if (v.isNull()){
		return Json::Value(Json::nullValue);
	}
	Poco::SharedPtr <IActionRW> a = IActionRW::Factory::instance().create(v->getName());
	IAction::Ptr vptr = v;
	Json::Value val = a->write(vptr);
	val["name"] = v->getName();
	return val;
}

IAction::Ptr ActionRW::read(const Json::Value & value) {
	if (value.isNull()){
		return IAction::Ptr();
	}
	Poco::SharedPtr <IActionRW> a = IActionRW::Factory::instance().create(value["name"].asString());
	return a->read(value);
}




Json::Value ConditionRW::write(const ICondition::Ptr & v) {
	Poco::SharedPtr <IConditionRW> a = IConditionRW::Factory::instance().create(v->getName());
	ICondition::Ptr vptr = v;
	Json::Value val = a->write(vptr);
	val["name"] = v->getName();
	return val;
}

ICondition::Ptr ConditionRW::read(const Json::Value & value) {
	Poco::SharedPtr <IConditionRW> a = IConditionRW::Factory::instance().create(value["name"].asString());
	return a->read(value);
}

} /* namespace BB */
