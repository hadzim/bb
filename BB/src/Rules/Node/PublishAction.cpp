/*
 * PublishAction.cpp
 *
 *  Created on: Nov 8, 2015
 *      Author: dron
 */

#include <BB/Rules/Node/PublishAction.h>

#include "BB/Node/INode.h"

#include "TBS/Log.h"

namespace BB {

PublishAction::PublishAction(std::string value, std::string topic, bool retain) :
	topic(topic), value(value), retain(retain){

}

PublishAction::~PublishAction() {

}

void PublishAction::perform(){
	LERROR("ACTION") << "Performing: " << topic << " value: " << value << LE;
	params.client->publish(topic, value, retain);
}

NewNodeSettingsAction::NewNodeSettingsAction(std::string value, std::string nodeId, std::string settingValue) :
		PublishAction(value, "devices/" + BB::Project::projectID() + "/nodes/" + nodeId + "/settings/" + settingValue + "/new"),
		nodeId(nodeId), settingValue(settingValue){

}


Json::Value PublishActionRW::twrite(ActionPtr & a){
	Json::Value v(Json::objectValue);
	v["topic"] = a->topic;
	v["value"] = a->value;
	v["retain"] = a->retain;
	return v;
}

IAction::Ptr PublishActionRW::read(const Json::Value & v){
	return new PublishAction(v["value"].asString(), v["topic"].asString(), v["retain"].asBool());
}

Json::Value NewNodeSettingsActionRW::twrite(ActionPtr & a){
	Json::Value v(Json::objectValue);
	v["nodeId"] = a->nodeId;
	v["setting"] = a->settingValue;
	v["value"] = a->value;
	return v;
}

IAction::Ptr NewNodeSettingsActionRW::read(const Json::Value & v){
	return new NewNodeSettingsAction(v["value"].asString(), v["nodeId"].asString(), v["setting"].asString());
}

REGISTER_ACTIONRW(PublishActionRW::getClassName(), PublishActionRW, PublishActionRW_)

REGISTER_ACTIONRW(NewNodeSettingsActionRW::getClassName(), NewNodeSettingsActionRW, NewNodeSettingsActionRW_)

} /* namespace BB */
