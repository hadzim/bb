/*
 * NodeHistory.cpp
 *
 *  Created on: Mar 5, 2015
 *      Author: dron
 */

#include <BB/Node/NodeHistory.h>
#include <TBS/Command.h>
#include <TBS/CommandNotification.h>

#include "BB/DataRW.h"

namespace BB {


	DataHistoryBuffer::DataHistoryBuffer(std::string name, Poco::Timespan span, int maxLength)
		: histName(name), span(span.totalMicroseconds()), maxLength(maxLength){

	}

	DataHistoryBuffer::BufferStatus DataHistoryBuffer::add(Node::Data data){
		bool shouldAdd =  lastTimestamp.isEmpty() || (lastTimestamp.ref().elapsed() >= span);
		if (!shouldAdd){
			return NotChanged;
		}

		lastTimestamp.set(Poco::Timestamp());
		buffer.push_back(data);

		while (buffer.size() > maxLength){
			buffer.pop_front();
		}

		return Changed;
	}

	DataHistoryBuffer::HistoricalData DataHistoryBuffer::data(){
		return buffer;
	}
	std::string DataHistoryBuffer::name(){
		return histName;
	}

	NodeHistory::NodeHistory(TBS::NotificationWorker::Ptr nw, TBS::MQTT::Client::Ptr client, std::string dataTopic):
		nw(nw), client(client), dataTopic(dataTopic){

		LDEBUG("History") << "new history " << dataTopic << LE;

		this->historicalData.push_back(
				DataHistoryBuffer("hour", Poco::Timespan(0,0,3,0,0), 20)
		);
		this->historicalData.push_back(
				DataHistoryBuffer("day", Poco::Timespan(0,0,30,0,0), 48)
		);
		this->historicalData.push_back(
				DataHistoryBuffer("week", Poco::Timespan(0,4,00,0,0), 6*7)
		);

		client->OnMessage += Poco::delegate(this, &NodeHistory::onMessage);
		//client->subscribe(dataTopic);

		LDEBUG("History") << "new history " << dataTopic << " done" << LE;

	}

	NodeHistory::~NodeHistory() {
		TBS::dumpBacktrace("History", "History", true);
		LDEBUG("History") << "finished history " << dataTopic << LE;

		//client->unsubscribe(dataTopic);
		client->OnMessage -= Poco::delegate(this, &NodeHistory::onMessage);
	}

	void NodeHistory::onMessage(TBS::MQTT::Message & message){
		//LDEBUG("History") << "accept in history " << dataTopic << LE;
		if (message.topic.topic() == dataTopic){

			LDEBUG("History") << "data accepted by " << dataTopic << LE;

			Node::Data val = NodeDataRW::read(RW::string2json(message.payload));
			TBS::ICommand::Ptr cmd = new TBS::CommandArgs<NodeHistory, Node::Data>("newdata", this, &NodeHistory::addData, val);
			TBS::CommandExecutorEnqueueAndContinue::execute(nw, cmd);
		}
	}

	void NodeHistory::addData(Node::Data data){
		for (auto & hist : this->historicalData){
			if (hist.add(data) == DataHistoryBuffer::Changed){


				auto allData = hist.data();

				Json::Value all(Json::arrayValue);
				for (auto d : allData){
					all.append(NodeDataRW::write(d));
				}




				std::string payload = RW::json2OneLine(all);
				std::string histTopic = dataTopic + "/history/" +  hist.name();

				LDEBUG("History") << "new data in: " << histTopic << " payload: " << payload << LE;

				client->publish(histTopic, payload);
			}
		}
	}

} /* namespace BB */
