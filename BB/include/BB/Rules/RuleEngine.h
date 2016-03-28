/*
 * RuleEngine.h
 *
 *  Created on: Nov 7, 2015
 *      Author: dron
 */

#ifndef RULEENGINE_H_
#define RULEENGINE_H_
#include <BB/Rules/Facts.h>
#include <BB/Rules/IAction.h>
#include <BB/Rules/Rules.h>
#include <Poco/SharedPtr.h>

namespace BB {

class RuleEngine {
public:
	typedef Poco::SharedPtr <RuleEngine> Ptr;
	RuleEngine(ActionParams params);
	virtual ~RuleEngine();

	void updateFact(IFact::Ptr fact);
	void removeFactByName(std::string factName);
	void removeFact(IFact::Ptr factName);

	void insertRule(Rule rule);
	void removeRuleByName(std::string ruleName);

	void save(std::string fileName);
	void load(std::string fileName);
private:
	void evaluate();
	void initRule(Rule & r);
private:
	ActionParams params;
	Facts facts;
	Rules rules;

};

} /* namespace BB */

#endif /* RULEENGINE_H_ */
