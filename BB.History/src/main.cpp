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
					client->subscribe(NodeChannel::eventLogTopic());

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

					if (msg.topic.topic() == NodeChannel::eventLogTopic()){
						LNOTICE("History") << "Adding new event log history: " << msg.payload << LE;
						INode::EventLogMessage evLogMsg = EventLogMessageRW::read(RW::string2json(msg.payload));
						TBS::ICommand::Ptr cmd = new TBS::CommandArgs<HistoryApp, INode::EventLogMessage>("neweventlog", this, &HistoryApp::onMessageImpl, evLogMsg);
						TBS::CommandExecutorEnqueueAndContinue::execute(nw, cmd);
						return;
					}

					if (buffers.find(t) == buffers.end()){
						LNOTICE("History") << "Adding new data stream: " << t << LE;
						buffers.insert(std::make_pair(t, new BB::NodeHistory(nw, client, t)));
					}


				}
				void onMessageImpl(INode::EventLogMessage msg){
					eventLog.push_front(msg);
					if (eventLog.size() > 50){
						eventLog.pop_back();
					}
					Json::Value val(Json::arrayValue);
					for (auto & m : eventLog){
						Json::Value v(EventLogMessageRW::write(m));
						val.append(v);
					}
					LNOTICE("History") << "Publish eventlog history: " << eventLog.size() << LE;
					client->publish(NodeChannel::eventLogTopic() + "/history", RW::json2OneLine(val));
					LNOTICE("History") << "Publish eventlog history: " << eventLog.size() << " done" << LE;
				}
			private:
				Poco::Mutex m;
				TBS::MQTT::Client::Ptr client;
				TBS::NotificationWorker::Ptr nw;
				std::map <std::string, BB::NodeHistory::Ptr> buffers;

				std::deque <INode::EventLogMessage> eventLog;
		};




} /* namespace BB */



int main(int argc, char** argv) {
	BB::HistoryApp app;
	return app.run(argc, argv);
}
