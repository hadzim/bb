/*
 * RuleEngine.cpp
 *
 *  Created on: Nov 7, 2015
 *      Author: dron
 */

#include <BB/Rules/RuleEngine.h>
#include <BB/Rules/Serialization.h>

namespace BB {

RuleEngine::RuleEngine(ActionParams params) : params(params) {

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

void RuleEngine::initRule(Rule & r){
	if (r.action()){
		r.action()->init(params);
	}
	if (r.negativeAction()){
		r.negativeAction()->init(params);
	}
}

void RuleEngine::insertRule(Rule rule){

	initRule(rule);

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
			if (r.second.action()){
				r.second.action()->perform();
			}
		} else {
			if (r.second.negativeAction()){
				r.second.negativeAction()->perform();
			}
		}
	}
}

void RuleEngine::save(std::string fileName){
	auto json = RulesRW::write(rules);
	RW::json2file(fileName, json);
}

void RuleEngine::load(std::string fileName){
	auto json = RW::file2json(fileName);
	rules = RulesRW::read(json);
	for (auto & r : rules){
		initRule(r.second);
	}
	evaluate();
}

} /* namespace BB */
