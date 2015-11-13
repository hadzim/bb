/*
 * ISensor.h
 *
 *  Created on: Feb 1, 2013
 *      Author: root
 */

#ifndef NODEMANAGER_H_
#define NODEMANAGER_H_
#include <BB/Node/NodeTypes.h>
#include "Poco/SharedPtr.h"
#include <vector>
#include <string>

#include "BB/DataRW.h"

#include "TBS/MQTT/Client.h"

#include "BB/Node/INode.h"

#include "TBS/SimpleTimer.h"

namespace BB {


	class NodeManager {
		public:

			typedef Poco::SharedPtr <NodeManager> Ptr;

			NodeManager(INode::Ptr node);
			~NodeManager();
		private:

			void load();
			void save();

			void onChanged(INode::SettingsValues & v);
			void onTimer(TBS::SimpleTimer::TimerArg & arg);
			void onMessage(TBS::MQTT::Message & arg);
			void onData(bool & arg);

			void read();
		private:
			TBS::MQTT::Client::Ptr client;
			INode::Ptr node;

			TBS::SimpleTimer timer;
	};

} /* namespace TBS */
#endif /* ISENSOR_H_ */
