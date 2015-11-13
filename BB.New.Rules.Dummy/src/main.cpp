#include <BB/Node/Listener/NodeListener.h>
#include <BB/Rules/Conditions/NotCondition.h>
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

class DummyAction : public IAction {
	public:
		std::string name;
		DummyAction(std::string name) : name(name){

		}

		void perform(){
			LERROR("ACTION") << name << LE;
		}
};


} /* namespace BB */


int main(int argc, char **argv) {
	TBS::initLogs("rules", 5);

	 TBS::MQTT::Client::Ptr client = new  TBS::MQTT::Client("rules-collector-dummy");

	BB::RuleEngine::Ptr engine = new BB::RuleEngine();

	{
		BB::NodeTagCondition::NodeMatch node;
		node.uid.set("temp@1");
		BB::ICondition::Ptr condition = new BB::NodeTagCondition("extremelyLow", node);

		//define rules
		{
			auto a = new BB::MultipleAction();
			a->addAction(new BB::DummyAction("Temperature is low"));
			a->addAction(new BB::NewNodeSettingsAction(client, "1", "switch1"));
			BB::Rule rule("dump when ext-low", condition, a);
			engine->insertRule(rule);
		}
		{
			auto a = new BB::MultipleAction();
			a->addAction(new BB::DummyAction("Temperature is high"));
			a->addAction(new BB::NewNodeSettingsAction(client, "0", "switch1"));
			BB::Rule rule("dump when not ext-low", new BB::NotCondition(condition), a);
			engine->insertRule(rule);
		}
	}
	BB::NodeTagFactCollector::Ptr collector = new BB::NodeTagFactCollector(engine);
	BB::NodeListener::Ptr listener = new BB::NodeListener(collector, client);
	while (true){
		Poco::Thread::sleep(100);
	}
	return 1;
}
