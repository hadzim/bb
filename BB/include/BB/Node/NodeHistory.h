/*
 * NodeHistory.h
 *
 *  Created on: Mar 5, 2015
 *      Author: dron
 */

#ifndef NODEHISTORY_H_
#define NODEHISTORY_H_

#include <BB/Node/NodeTypes.h>
#include <TBS/NotificationWorker.h>

#include "TBS/Nullable.h"

#include "TBS/MQTT/Client.h"

namespace BB {

	class DataHistoryBuffer {
		public:

			typedef std::deque<Node::Data> HistoricalData;

			DataHistoryBuffer(std::string name, Poco::Timespan span, int maxLength);

			enum BufferStatus{
				Changed,
				NotChanged
			};

			BufferStatus add(Node::Data data);
			HistoricalData data();
			std::string name();
		private:

			std::string histName;
			Poco::Timestamp::TimeDiff span;
			HistoricalData buffer;
			TBS::Nullable<Poco::Timestamp> lastTimestamp;
			int maxLength;
	};

	class NodeHistory {
		public:
			typedef Poco::SharedPtr <NodeHistory> Ptr;
			NodeHistory(TBS::NotificationWorker::Ptr nw, TBS::MQTT::Client::Ptr client, std::string topic);
			~NodeHistory();

		private:
			void onMessage(TBS::MQTT::Message & message);

			void addData(Node::Data data);
		private:

			TBS::NotificationWorker::Ptr nw;
			TBS::MQTT::Client::Ptr client;
			std::string dataTopic;

			std::vector <DataHistoryBuffer> historicalData;
	};

} /* namespace BB */

#endif /* NODEHISTORY_H_ */
