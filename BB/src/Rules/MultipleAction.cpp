/*
 * MultipleAction.cpp
 *
 *  Created on: Nov 8, 2015
 *      Author: dron
 */

#include <BB/Rules/MultipleAction.h>
#include <BB/Rules/Serialization.h>

namespace BB {

MultipleAction::MultipleAction(Actions a) :
		actions(a) {

}

MultipleAction::~MultipleAction() {

}

void MultipleAction::addAction(IAction::Ptr action) {
	actions.push_back(action);
}

void MultipleAction::perform() {
	for (auto a : actions) {
		a->perform();
	}
}
void MultipleAction::init(ActionParams params){
	for (auto a : actions) {
		a->init(params);
	}
}

Json::Value MultipleActionRW::twrite(ActionPtr & a){
	Json::Value v(Json::objectValue);
	v["actions"] = Json::arrayValue;
	for (auto action : a->actions){
		v["actions"].append(ActionRW::write(action));
	}
	return v;
}
IAction::Ptr MultipleActionRW::read(const Json::Value & value){
	MultipleAction::Ptr ma = new MultipleAction();
	for (auto jv : value["actions"]){
		ma->addAction(ActionRW::read(jv));
	}
	return ma;
}

REGISTER_ACTIONRW(MultipleActionRW::getClassName(), MultipleActionRW, MultipleActionRW_)

} /* namespace BB */


