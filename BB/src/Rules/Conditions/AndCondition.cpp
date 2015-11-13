/*
 * AndCondition.cpp
 *
 *  Created on: Nov 11, 2015
 *      Author: dron
 */

#include <BB/Rules/Conditions/AndCondition.h>

namespace BB {

AndCondition::AndCondition(ICondition::Ptr c1, ICondition::Ptr c2){
	conditions.push_back(c1);
	conditions.push_back(c2);
}
AndCondition::AndCondition(std::vector <ICondition::Ptr> conditions) : conditions(conditions){

}

bool AndCondition::isValid(const Facts & facts){
	for (auto c : conditions){
		if (!c->isValid(facts)) return false;
	}
	return true;
}

} /* namespace BB */
