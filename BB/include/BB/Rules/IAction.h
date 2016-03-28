/*
 * IAction.h
 *
 *  Created on: Nov 7, 2015
 *      Author: dron
 */

#ifndef IACTION_H_
#define IACTION_H_
#include <Poco/SharedPtr.h>
#include "TBS/NamedObject.h"

#include "TBS/MQTT/Client.h"

namespace BB {

struct ActionParams {
	TBS::MQTT::Client::Ptr client;
};

class IAction {
public:

	typedef Poco::SharedPtr <IAction> Ptr;

	IAction();
	virtual ~IAction();

	virtual void perform() = 0;
	virtual void init(ActionParams params);

	ABSTRACT_NAMED_OBJECT

protected:
	ActionParams params;
};

} /* namespace BB */

#endif /* IACTION_H_ */
