/*
 * main.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#include "BB/App2.h"
#include "BB/Node/NodeHistory.h"
#include <map>

#include "TBS/MQTT/Client.h"
#include "TBS/Command.h"
#include "TBS/NotificationWorker.h"
#include "TBS/CommandNotification.h"

#include "BB/ServiceNotification.h"

#include "TBS/Log.h"

namespace BB {

	class HistoryApp: public Poco::Util::ServerApplication {

			public:
				typedef Poco::SharedPtr<NodeApp> Ptr;
				HistoryApp(){
					nw = new TBS::NotificationWorker("BB.History");

					client = new TBS::MQTT::Client("history-service");
					client->OnMessage += Poco::delegate(this, &HistoryApp::onMessage);
					client->subscribe("devices/+/nodes/+/data/+");

				}
				virtual ~HistoryApp(){
					client->OnMessage -= Poco::delegate(this, &HistoryApp::onMessage);

					buffers.clear();

					client = NULL;

					nw = NULL;
				}

			protected:

				int main(const std::vector<std::string>& args){

					TBS::initLogs("history", 8, "/tmp/");

					BB::ServiceNotification::serviceReady();

					waitForTerminationRequest();

					BB::ServiceNotification::serviceDisabled("stopped");
					return 0;
				}
			private:
				void onMessage(TBS::MQTT::Message & msg){
					Poco::Mutex::ScopedLock l(m);
					std::string t = msg.topic.topic();
					if (buffers.find(t) == buffers.end()){
						LNOTICE("History") << "Adding new data stream: " << t << LE;
						buffers.insert(std::make_pair(t, new BB::NodeHistory(nw, client, t)));
					}
				}
				void onMessageImpl(std::string topic){

				}
			private:
				Poco::Mutex m;
				TBS::MQTT::Client::Ptr client;
				TBS::NotificationWorker::Ptr nw;
				std::map <std::string, BB::NodeHistory::Ptr> buffers;
		};




} /* namespace BB */



int main(int argc, char** argv) {
	BB::HistoryApp app;
	return app.run(argc, argv);
}
