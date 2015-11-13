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

PublishAction::PublishAction(TBS::MQTT::Client::Ptr client, std::string value, std::string topic, bool retain) :
	client(client), topic(topic), value(value), retain(retain){

}

PublishAction::~PublishAction() {

}

void PublishAction::perform(){
	LERROR("ACTION") << "Performing: " << topic << " value: " << value << LE;
	client->publish(topic, value, retain);
}

NewNodeSettingsAction::NewNodeSettingsAction(TBS::MQTT::Client::Ptr client, std::string value, std::string nodeId, std::string settingValue) :
		PublishAction(client, value, "devices/" + BB::Project::projectID() + "/nodes/" + nodeId + "/settings/" + settingValue + "/new"){

}


} /* namespace BB */
