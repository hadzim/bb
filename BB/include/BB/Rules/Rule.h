/*
 * Rule.h
 *
 *  Created on: Nov 7, 2015
 *      Author: dron
 */

#ifndef RULE_H_
#define RULE_H_
#include <BB/Rules/IAction.h>
#include <BB/Rules/ICondition.h>

namespace BB {

class Rule {
public:
	Rule(std::string name, ICondition::Ptr condition, IAction::Ptr action, IAction::Ptr negativeAction = IAction::Ptr());
	virtual ~Rule();

	const std::string name() const;

	ICondition::Ptr condition();
	IAction::Ptr action();
	IAction::Ptr negativeAction();
private:
	std::string name_;

	ICondition::Ptr condition_;
	IAction::Ptr action_;
	IAction::Ptr negativeAction_;
};

} /* namespace BB */

#endif /* RULE_H_ */
