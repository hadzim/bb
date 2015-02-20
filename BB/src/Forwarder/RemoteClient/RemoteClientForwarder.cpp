/*
 * RemoteClientForwarder.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#include <BB/Service/DataSender.h>
#include "BB/Forwarder/RemoteClient/RemoteClientForwarder.h"
#include <iostream>
#include <Poco/Delegate.h>
#include "Poco/Net/HTMLForm.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "BB/DataRW.h"
#include "TBS/Log.h"
#include <fstream>

#include "Poco/URI.h"

#include "json/value.h"

#include "json/reader.h"
#include "BB/Services/DataSvc_DBus.h"

namespace BB {

	RemoteClientForwarder::RemoteClientForwarder(const RemoteClientSettings & settings) :
			client(settings), stopped(false), t("taskaccept") {

		LDEBUG("info")<< "create client:" << settings.url << " port" << settings.port << " q: " << settings.query << LE;

			t.start(*this);
		}

	RemoteClientForwarder::~RemoteClientForwarder() {
		stopped = true;
		t.join();
	}

	void RemoteClientForwarder::forward(const SensorData & m) {
		client.forward(m);

	}
	void RemoteClientForwarder::forward(const RuntimeStatus & data) {
		client.forward(data);

	}
	void RemoteClientForwarder::forward(const Task & data) {

	}
	void RemoteClientForwarder::forward(const Notification & data) {
		client.forward(data);
	}

	Task::Map readAll(const std::string & s) {
		Task::Map all;

		Json::Value v;
		Json::Reader r;
		r.parse(s, v);

		if (v.isArray()) {
			for (Json::Value::iterator it = v.begin(); it != v.end(); it++) {
				all.insert(std::make_pair((*it)["id"].asInt(), TaskDataRW::read(*it)));
			}
		}

		return all;
	}

	void RemoteClientForwarder::run() {

		LDEBUG("info")<<"start bg" << LE;

		DataSender sender;

		while (!stopped) {
			try {
				for (int i = 0; i < 60; i++) {
					if (stopped) {
						break;
					}
					Poco::Thread::sleep(1000);
				}

				if (stopped) {
					break;
				}

				//ask for tasks
				std::string res = this->client.remoteQuery("tasks", "askForData");
				std::ofstream f("/tmp/lastTask.txt");
				f << res;

				LDEBUG("info") << "result:" << res << LE;

				f << "\n\n";
				f << "\n\n";
				try {
					Task::Map tasks = readAll(res);
					for (Task::Map::iterator it = tasks.begin(); it != tasks.end(); it++) {
						try {

							LNOTICE("tasks") <<"new task: " << it->second << LE;

							f << it->second << std::endl;

							sender.send(it->second);

							std::stringstream s;
							s << "[" << it->first << "]";

							std::string cfres = this->client.remoteQuery("confirm", s.str());
							f << "confirm res: " << cfres << std::endl;
							LDEBUG("tasks") <<"ok: " << cfres << LE;
						} catch (Poco::Exception & e) {
							LERROR("tasks") <<"exc: " << e.displayText() << LE;
							LERROR("tasks") <<"exc: " << e.displayText() << LE;
						}
					}
					f << "done" << std::endl;
				} catch (Poco::Exception & e) {
					LERROR("tasks")<<"proc exc: " << e.displayText() << LE;
				} catch (std::runtime_error & e) {
					LERROR("tasks") << "proc exc: " << e.what() << LE;
				} catch (...) {
					LERROR("tasks") << "proc exc: " << LE;
				}


			} catch (Poco::Exception & e) {
				LERROR("tasks")<<"q exc: " << e.displayText() << LE;
			} catch (std::runtime_error & e) {
				LERROR("tasks") << "q exc: " << e.what() << LE;
			} catch (...) {
				LERROR("tasks") << "q exc: " << LE;
			}
		}

	}

}
/* namespace BB */
