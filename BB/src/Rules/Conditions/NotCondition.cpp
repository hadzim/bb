/*
 * NotCondition.cpp
 *
 *  Created on: Nov 11, 2015
 *      Author: dron
 */

#include <BB/Rules/Conditions/NotCondition.h>

namespace BB {

NotCondition::NotCondition(ICondition::Ptr c) : c(c) {

}

bool NotCondition::isValid(const Facts & facts){
	return !c->isValid(facts);
}

} /* namespace BB */
