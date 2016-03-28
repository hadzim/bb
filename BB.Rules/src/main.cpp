#include <BB/Node/Listener/NodeListener.h>
#include <BB/Rules/Conditions/NotCondition.h>
#include <BB/Rules/Conditions/OrCondition.h>
#include <BB/Rules/MultipleAction.h>
#include <BB/Rules/Node/NodeTagCondition.h>
#include <BB/Rules/Node/NodeTagFactCollector.h>
#include <BB/Rules/Node/PublishAction.h>
#include <BB/Rules/Rule.h>
#include <BB/Rules/RuleEngine.h>
#include <BB/ServiceNotification.h>
#include "TBS/Log.h"

#include "Poco/Thread.h"

#include "BB/Node/INode.h"

#include "DefaultRules.h"
#include "Poco/File.h"

#include "Poco/Util/ServerApplication.h"

class RulesApp: public Poco::Util::ServerApplication {

		int main(const std::vector<std::string>& args) {
			try {
				TBS::initLogs("rules", 5, "/tmp/");

				TBS::MQTT::Client::Ptr client = new TBS::MQTT::Client("rules-collector-dummy");

				BB::ActionParams p;
				p.client = client;

				BB::RuleEngine::Ptr engine = new BB::RuleEngine(p);

				std::string ruleFile = "data/rules.json";
#ifdef TBS_TPRO
				ruleFile = "/home/tbs/data/rules.json";
#endif

				{
					BB::RuleEngine::Ptr tmpEngine = new BB::RuleEngine(p);
					BB::initDefaultRules(tmpEngine);
					tmpEngine->save(ruleFile + ".default");
					if (!Poco::File(ruleFile).exists()) {
						tmpEngine->save(ruleFile);
					}
				}

				std::cout << "LOAD FROM: " << ruleFile << std::endl;

				engine->load(ruleFile);

				BB::NodeTagFactCollector::Ptr collector = new BB::NodeTagFactCollector(engine);
				BB::NodeListener::Ptr listener = new BB::NodeListener(collector, client);

				BB::ServiceNotification::serviceReady();

				this->waitForTerminationRequest();

				BB::ServiceNotification::serviceDisabled();

			} catch (Poco::Exception & e) {
				LERROR("EXC:")<< e.displayText() << LE;
				}

				return EXIT_OK;
			}
		};

int main(int argc, char** argv) {
	RulesApp app;
	return app.run(argc, argv);
}

