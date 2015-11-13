/*
 * NotCondition.h
 *
 *  Created on: Nov 11, 2015
 *      Author: dron
 */

#ifndef NOTCONDITION_H_
#define NOTCONDITION_H_
#include <BB/Rules/ICondition.h>

namespace BB {

class NotCondition : public ICondition {
public:
	NotCondition(ICondition::Ptr c);
	virtual bool isValid(const Facts & facts);
private:
	ICondition::Ptr c;
};

} /* namespace BB */

#endif /* NOTCONDITION_H_ */
