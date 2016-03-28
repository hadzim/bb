/*
 * SensorData.h
 *
 *  Created on: 18.3.2014
 *      Author: JV
 */

#ifndef SENSORDATARW_H_
#define SENSORDATARW_H_


#include <BB/Notification.h>
#include <BB/RuntimeStatus.h>
#include <BB/Sensor/SensorData2.h>
#include <BB/Task/Task.h>
#include <string>

#include <iostream>
#include "json/value.h"
#include "BB/Sensor/SensorData.h"
#include "BB/Node/NodeTypes.h"
#include "BB/RW.h"

#include "BB/Node/INode.h"

namespace BB {


	class SensorDataRW {
		public:
			static Json::Value write(const SensorData & sensorData);
			static SensorData read(const Json::Value & value);
	};

	class Sensor2DataRW {
		public:
			static Json::Value write(const Sensor::Data & sensorData);
			static Sensor::Data read(const Json::Value & value);
	};

	class NodeDataRW {
		public:
			static Json::Value write(const Node::Data & sensorData);
			static Node::Data read(const Json::Value & value);
	};

	class NodeInfoRW {
		public:
			static Json::Value write(const Node::Info & sensorData);
			static Node::Info read(const Json::Value & value);
	};

	class NodeSensorRW {
		public:
			static Json::Value write(const Node::Sensor & sensorData);
			static Node::Sensor read(const Json::Value & value);
	};

	class NodeSettingsRW {
		public:
			static Json::Value write(const Node::Setting & sensorData);
			static Node::Setting read(const Json::Value & value);
	};

	class StatusDataRW {
		public:
			static Json::Value write(const RuntimeStatus & status);
			static RuntimeStatus read(const Json::Value & value);
	};

	class TaskDataRW {
		public:
			static Json::Value write(const Task & status);
			static Task read(const Json::Value & value);
	};

	class NotificationDataRW {
		public:
			static Json::Value write(const Notification & ntf);
			static Notification read(const Json::Value & value);
	};

	class EventLogMessageRW {
		public:
			static Json::Value write(const INode::EventLogMessage & ntf);
			static INode::EventLogMessage read(const Json::Value & value);
	};

}

#endif /* SENSORDATA_H_ */
