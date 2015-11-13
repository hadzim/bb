/*
 * MQTT.h
 *
 *  Created on: Nov 27, 2014
 *      Author: dron
 */

#ifndef MQTTCLient_H_
#define MQTTCLient_H_

#include <Poco/BasicEvent.h>
#include <Poco/Runnable.h>
#include <Poco/Thread.h>

#include "TBS/MQTT/Types.h"

namespace TBS {
	namespace MQTT {
		class MQTTImpl;

		class Client: private Poco::Runnable {
			public:
				typedef Poco::SharedPtr <Client> Ptr;

				Client(std::string id, std::string host = "localhost", int port = 1883);
				~Client();

				void subscribe(std::string topic);
				void unsubscribe(std::string topic);
				void publish(std::string topic, std::string payload, bool retain = true, QoS::QoSType qos = QoS::NoConfirmation);

				MessageEvent OnMessage;
				SubscribeEvent OnSubscribe;
				ConnectEvent OnConnect;
			private:
				void run();

				Client(const Client &);
				Client & operator=(const Client &);
			private:
				static int instances;
				static Poco::Mutex instancesM;

				MQTTImpl * impl;

				Poco::Thread t;
				bool stopped;
				Poco::Mutex m;

		};
	}
} /* namespace TBS */

#endif /* MQTT_H_ */
