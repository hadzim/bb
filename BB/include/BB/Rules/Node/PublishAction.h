/*
 * PublishAction.h
 *
 *  Created on: Nov 8, 2015
 *      Author: dron
 */

#ifndef PUBLISHACTION_H_
#define PUBLISHACTION_H_
#include <BB/Rules/IAction.h>
#include <BB/Rules/Serialization.h>
#include <TBS/MQTT/Client.h>

namespace BB {

class PublishActionRW;
class NewNodeSettingsActionRW;

class PublishAction : public IAction {
public:
	typedef Poco::SharedPtr <PublishAction> Ptr;


	PublishAction(std::string value, std::string topic, bool retain = false);
	virtual ~PublishAction();

	virtual void perform();

	NAMED_OBJECT("Publish")

protected:

	std::string value;
	std::string topic;
	bool retain;

	friend class PublishActionRW;
};

class PublishActionRW : public TActionRW <PublishAction> {
public:
	virtual Json::Value twrite(ActionPtr & a);
	virtual IAction::Ptr read(const Json::Value & value);

	NAMED_OBJECT(PublishAction::getClassName())
};

class NewNodeSettingsAction : public PublishAction {
public:
	typedef Poco::SharedPtr <NewNodeSettingsAction> Ptr;

	NewNodeSettingsAction(std::string value, std::string nodeId, std::string settingValue = "valueAuto");

	NAMED_OBJECT("NewNodeSettings")

	friend class NewNodeSettingsActionRW;
private:
	std::string nodeId;
	std::string settingValue;

};

class NewNodeSettingsActionRW : public TActionRW <NewNodeSettingsAction> {
public:
	virtual Json::Value twrite(ActionPtr & a);
	virtual IAction::Ptr read(const Json::Value & value);

	NAMED_OBJECT(NewNodeSettingsAction::getClassName())
};

} /* namespace BB */

#endif /* PUBLISHACTION_H_ */
