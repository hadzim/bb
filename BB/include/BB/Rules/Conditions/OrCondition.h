/*
 * OrCondition.h
 *
 *  Created on: Nov 11, 2015
 *      Author: dron
 */

#ifndef ORCONDITION_H_
#define ORCONDITION_H_
#include <BB/Rules/ICondition.h>

namespace BB {

class OrCondition : public ICondition {
public:
	typedef Poco::SharedPtr <ICondition> Ptr;

	OrCondition(ICondition::Ptr c1, ICondition::Ptr c2);
	OrCondition(std::vector <ICondition::Ptr> conditions);

	virtual bool isValid(const Facts & facts);
private:
	std::vector <ICondition::Ptr> conditions;
};

} /* namespace BB */

#endif /* ORCONDITION_H_ */
