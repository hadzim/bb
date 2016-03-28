/*
 * ICondition.h
 *
 *  Created on: Nov 7, 2015
 *      Author: dron
 */

#ifndef ICONDITION_H_
#define ICONDITION_H_
#include <BB/Rules/Facts.h>
#include <Poco/SharedPtr.h>
#include "TBS/NamedObject.h"

namespace BB {

class ICondition  {
public:
	typedef Poco::SharedPtr <ICondition> Ptr;
	ICondition();
	virtual ~ICondition();

	virtual bool isValid(const Facts & facts) = 0;

	ABSTRACT_NAMED_OBJECT
};

} /* namespace BB */

#endif /* ICONDITION_H_ */
