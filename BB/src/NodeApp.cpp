#include <BB/ServiceNotification.h>
#include "BB/NodeApp.h"
#include <iostream>
#include <Poco/Delegate.h>
#include "Poco/Thread.h"
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

		timer.start(factory->getCheckingPeriodInMs() / 20, factory->getCheckingPeriodInMs());

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

			std::cout << "factory timer" << std::endl;

			auto newnodes = factory->getNodes();

			std::cout << "factory timer" << std::endl;

			for (auto & n : newnodes) {
				std::cout << "factory scan: " << n->getInfo().getUID() << std::endl;
				std::string key = n->getInfo().getUID();
				if (nodes.find(key) == nodes.end()){
					std::cout << "insert new: " << n->getInfo().getUID() << std::endl;

					nodes.insert(std::make_pair(key, new NodeManager(n)));
				}
			}
		} catch (Poco::Exception & e) {
			LERROR("Sensor.App")<< "factory Poco exc: " << e.displayText() << LE;
		} catch (std::exception & e) {
			LERROR("Sensor.App")<< "factory std exc: " << e.what() << LE;
		}
	}

}
/* namespace BB */
