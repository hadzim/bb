/*
 * AndCondition.h
 *
 *  Created on: Nov 11, 2015
 *      Author: dron
 */

#ifndef ANDCONDITION_H_
#define ANDCONDITION_H_
#include <BB/Rules/ICondition.h>
#include <vector>

namespace BB {

class AndCondition : public ICondition {
public:
	typedef Poco::SharedPtr <ICondition> Ptr;

	AndCondition(ICondition::Ptr c1, ICondition::Ptr c2);
	AndCondition(std::vector <ICondition::Ptr> conditions);

	virtual bool isValid(const Facts & facts);
private:
	std::vector <ICondition::Ptr> conditions;
};

} /* namespace BB */

#endif /* ANDCONDITION_H_ */
