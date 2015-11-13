/*
 * IAction.h
 *
 *  Created on: Nov 7, 2015
 *      Author: dron
 */

#ifndef IACTION_H_
#define IACTION_H_
#include <Poco/SharedPtr.h>

namespace BB {

class IAction {
public:

	typedef Poco::SharedPtr <IAction> Ptr;

	IAction();
	virtual ~IAction();

	virtual void perform() = 0;
};

} /* namespace BB */

#endif /* IACTION_H_ */
