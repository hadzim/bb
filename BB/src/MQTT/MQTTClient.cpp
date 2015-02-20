/*
 * MQTT.cpp
 *
 *  Created on: Nov 27, 2014
 *      Author: dron
 */

#include "MQTT/MQTTClient.h"

#include <mosquittopp.h>
#include <iostream>

#include "TBS/Log.h"

namespace TBS {
	namespace MQTT {
		class MQTTImpl: public mosqpp::mosquittopp {
			public:
				MQTTImpl(std::string id, std::string host, int port, MessageEvent & msg, SubscribeEvent & sub, ConnectEvent & cn) :
						mosqpp::mosquittopp(id.c_str()), msg(msg), sub(sub), cn(cn) {
					LDEBUG("MQTT") << "Connect to: " << host << " port " << port << LE;
					int keepalive = 60;
					this->connect(host.c_str(), port, keepalive);
				}
				virtual ~MQTTImpl() {
					this->disconnect();
				}

				void on_connect(int rc) {
					ConnectStatus s = (rc == 0) ? ConnectOk : ConnectFailed;
					cn.notify(this, s);
				}
				void on_message(const struct mosquitto_message *message) {
					Message newmessage(
							message->mid,
							Topic(message->topic),
							std::string((char *) message->payload, message->payloadlen),
							(QoS::QoSType)message->qos,
							message->retain
					);
					try {
						msg.notify(this, newmessage);
					} catch (std::exception & e) {
						LERROR("MQTT") << "Error on message: " << e.what() << LE;
						throw;
					}
				}
				void on_subscribe(int mid, int qos_count, const int *granted_qos) {
					Subscribe s;
					s.mid = mid;
					s.qosCount = qos_count;
					//TODO granted QOS
					sub.notify(this, s);
				}
			private:
				MessageEvent & msg;
				SubscribeEvent & sub;
				ConnectEvent & cn;
		};

		MQTTClient::MQTTClient(std::string id, std::string host, int port) :
				stopped(false) {
			//static init
			mosqpp::lib_init();

			//tempconv = new mqtt_tempconv("tempconv", "localhost", 1883);
			impl = new MQTTImpl(id, host, port, OnMessage, OnSubscribe, OnConnect);
			t.start(*this);
		}

		MQTTClient::~MQTTClient() {
			stopped = true;
			t.join();
			//static uninit
			mosqpp::lib_cleanup();
		}

		void MQTTClient::subscribe(std::string topic) {
			LTRACE("MQTT") << " subs before lock" << LE;
			Poco::Mutex::ScopedLock l(m);
			LTRACE("MQTT") << " subs after lock" << LE;
			impl->subscribe(NULL, topic.c_str());
			LTRACE("MQTT") << " subs done" << LE;
		}
		void MQTTClient::publish(std::string topic, std::string payload, bool retain, QoS::QoSType qos) {
			LTRACE("MQTT") << "going to publish: " << topic << " before lock" << LE;
			Poco::Mutex::ScopedLock l(m);
			LTRACE("MQTT") << "going to publish: " << topic << " after lock" << LE;
			impl->publish(NULL, topic.c_str(), payload.size(), payload.c_str(), (int)qos, retain);
			LTRACE("MQTT") << "going to publish: " << topic << " done" << LE;
		}

		void MQTTClient::run() {
			while (!stopped) {
				int rc = 0;
				{
					LTRACE("MQTT") << "loop internally" << LE;
					Poco::Mutex::ScopedLock l(m);
					LTRACE("MQTT") << "loop internally locked" << LE;
					rc = impl->loop();
					LTRACE("MQTT") << "loop internally done" << LE;
				}
				if (rc) {
					Poco::Mutex::ScopedLock l(m);
					impl->reconnect();
				}
				Poco::Thread::sleep(5);
			}
		}
	}
} /* namespace TBS */
