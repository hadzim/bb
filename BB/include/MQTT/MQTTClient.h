/*
 * MQTT.h
 *
 *  Created on: Nov 27, 2014
 *      Author: dron
 */

#ifndef MQTT_H_
#define MQTT_H_

#include <Poco/BasicEvent.h>
#include <Poco/Runnable.h>
#include <Poco/Thread.h>

#include "Types.h"

namespace TBS {
	namespace MQTT {
		class MQTTImpl;

		class MQTTClient: private Poco::Runnable {
			public:
				typedef Poco::SharedPtr <MQTTClient> Ptr;

				MQTTClient(std::string id, std::string host = "localhost", int port = 1883);
				~MQTTClient();

				void subscribe(std::string topic);
				void publish(std::string topic, std::string payload, bool retain = true, QoS::QoSType qos = QoS::NoConfirmation);

				MessageEvent OnMessage;
				SubscribeEvent OnSubscribe;
				ConnectEvent OnConnect;
			private:
				void run();
			private:

				MQTTImpl * impl;

				Poco::Thread t;
				bool stopped;
				Poco::Mutex m;

		};
	}
} /* namespace TBS */

#endif /* MQTT_H_ */
