/*
 * RuleEngine.cpp
 *
 *  Created on: Nov 7, 2015
 *      Author: dron
 */

#include <BB/Rules/RuleEngine.h>

namespace BB {

RuleEngine::RuleEngine() {

}

RuleEngine::~RuleEngine() {

}

void RuleEngine::updateFact(IFact::Ptr fact){
	facts[fact->name()] = fact;
	evaluate();
}
void RuleEngine::removeFactByName(std::string factName){
	facts.erase(factName);
	evaluate();
}
void RuleEngine::removeFact(IFact::Ptr fact){
	facts.erase(fact->name());
	evaluate();
}

void RuleEngine::insertRule(Rule rule){
	rules.erase(rule.name());
	rules.insert(std::make_pair(rule.name(), rule));
	evaluate();
}
void RuleEngine::removeRuleByName(std::string ruleName){
	rules.erase(ruleName);
	evaluate();
}

void RuleEngine::evaluate(){
	for (auto r : rules){
		if (r.second.condition()->isValid(facts)){
			r.second.action()->perform();
		}
	}
}


} /* namespace BB */
