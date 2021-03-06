/*
 * Rule.cpp
 *
 *  Created on: Nov 7, 2015
 *      Author: dron
 */

#include <BB/Rules/Rule.h>

namespace BB {

Rule::Rule(std::string name, ICondition::Ptr condition, IAction::Ptr action, IAction::Ptr negativeAction)
	: name_(name), condition_(condition), action_(action), negativeAction_(negativeAction) {

}

Rule::~Rule() {

}

const std::string Rule::name() const {
	return name_;
}


ICondition::Ptr Rule::condition(){
	return condition_;
}
IAction::Ptr Rule::action(){
	return action_;
}
IAction::Ptr Rule::negativeAction(){
	return negativeAction_;
}

} /* namespace BB */
