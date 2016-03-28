/*
 * DefaultRules.h
 *
 *  Created on: Nov 17, 2015
 *      Author: dron
 */

#ifndef DEFAULTRULES_H_
#define DEFAULTRULES_H_

#include <BB/Node/Listener/NodeListener.h>
#include <BB/Rules/Conditions/AndCondition.h>
#include <BB/Rules/Conditions/NotCondition.h>
#include <BB/Rules/Conditions/OrCondition.h>
#include <BB/Rules/MultipleAction.h>
#include <BB/Rules/Node/NodeTagCondition.h>
#include <BB/Rules/Node/NodeTagFactCollector.h>
#include <BB/Rules/Node/PublishAction.h>
#include <BB/Rules/Rule.h>
#include <BB/Rules/RuleEngine.h>
#include "TBS/Log.h"

#include "Poco/Thread.h"

#include "BB/Node/INode.h"

namespace BB {

	void initDefaultRules(RuleEngine::Ptr engine) {

		//THERMOSTAT
		{

			BB::NodeTagCondition::NodeMatch node;
			node.uid.set("temp@1");
			BB::ICondition::Ptr condition = new BB::NodeTagCondition("extremelyLow", node);



			//define rules
			{
				auto a = new BB::NewNodeSettingsAction("1", "switch1");
				auto na = new BB::NewNodeSettingsAction("0", "switch1");

				BB::Rule rule("Thermostat", condition, a, na);
				engine->insertRule(rule);
			}
		}

		//ALARM
		{

			BB::ICondition::Ptr openCondition = new BB::NodeTagCondition("opened[s]");
			BB::ICondition::Ptr motionCondition = new BB::NodeTagCondition("motion[s]");
			BB::ICondition::Ptr armedCondition = new BB::NodeTagCondition("armed");

			BB::ICondition::Ptr orC = new OrCondition(openCondition, motionCondition);
			BB::ICondition::Ptr andC = new AndCondition(armedCondition, orC);

			//define rules
			{
				auto a = new BB::NewNodeSettingsAction("1", "security", "alarmAuto");
				auto na = new BB::NewNodeSettingsAction("0", "security", "alarmAuto");

				BB::Rule rule("Alarm", andC, a, na);
				engine->insertRule(rule);
			}
		}

	}

}

#endif /* DEFAULTRULES_H_ */
