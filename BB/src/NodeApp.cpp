#include <BB/ServiceNotification.h>
#include "BB/NodeApp.h"
#include <iostream>
#include <Poco/Delegate.h>
#include "Poco/Thread.h"
#include "BB/Sensor/Net/Forecast.h"
#include "BB/Sensor/SensorDataHelpers.h"
#include "BB/Services/DataSvc_DBus.h"

#include "TBS/Log.h"

namespace BB {

	NodeApp::NodeApp(std::string name, INodeFactory::Ptr factory) :
			name(name), factory(factory) {
		timer.Timer += Poco::delegate(this, &NodeApp::onTimer);
	}

	NodeApp::~NodeApp() {
		timer.Timer -= Poco::delegate(this, &NodeApp::onTimer);
	}

	int NodeApp::main(const std::vector<std::string>& args) {

		std::cout << "enter main: " << name << std::endl;

		TBS::initLogs(name, 8, "/tmp/");

		timer.start(factory->getPeriodInMs() / 20, factory->getPeriodInMs());

		std::cout << "main SensorApp2.start" << std::endl;

		BB::ServiceNotification::serviceReady();

		this->waitForTerminationRequest();

		BB::ServiceNotification::serviceDisabled();

		timer.stop();

		std::cout << "main SensorApp2.stop" << std::endl;

		return EXIT_OK;
	}
	void NodeApp::onTimer(TBS::SimpleTimer::TimerArg & t) {
		try {
			//get new nodes
			auto newnodes = factory->getNodes();

			for (auto & n : newnodes) {
				std::string key = n->getInfo().getUID();
				if (nodes.find(key) == nodes.end()){
					nodes.insert(std::make_pair(key, NodeManager(n)));
				}
			}
		} catch (Poco::Exception & e) {
			LERROR("Sensor.App")<<e.displayText() << LE;
		}
	}

}
/* namespace BB */
