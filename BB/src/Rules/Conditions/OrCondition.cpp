/*
 * OrCondition.cpp
 *
 *  Created on: Nov 11, 2015
 *      Author: dron
 */

#include <BB/Rules/Conditions/OrCondition.h>

namespace BB {


OrCondition::OrCondition(ICondition::Ptr c1, ICondition::Ptr c2){
	conditions.push_back(c1);
	conditions.push_back(c2);
}
OrCondition::OrCondition(std::vector <ICondition::Ptr> conditions) : conditions(conditions){

}

bool OrCondition::isValid(const Facts & facts){
	for (auto c : conditions){
		if (c->isValid(facts)) return true;
	}
	return false;
}

} /* namespace BB */
