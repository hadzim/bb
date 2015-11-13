/*
 * PublishAction.h
 *
 *  Created on: Nov 8, 2015
 *      Author: dron
 */

#ifndef PUBLISHACTION_H_
#define PUBLISHACTION_H_
#include <BB/Rules/IAction.h>
#include <TBS/MQTT/Client.h>

namespace BB {

class PublishAction : public IAction {
public:
	typedef Poco::SharedPtr <PublishAction> Ptr;


	PublishAction(TBS::MQTT::Client::Ptr client, std::string value, std::string topic, bool retain = false);
	virtual ~PublishAction();

	virtual void perform();

private:
	TBS::MQTT::Client::Ptr client;
	std::string value;
	std::string topic;
	bool retain;

};

class NewNodeSettingsAction : public PublishAction {
public:
	typedef Poco::SharedPtr <NewNodeSettingsAction> Ptr;

	NewNodeSettingsAction(TBS::MQTT::Client::Ptr client, std::string value, std::string nodeId, std::string settingValue = "valueAuto");

};


} /* namespace BB */

#endif /* PUBLISHACTION_H_ */
