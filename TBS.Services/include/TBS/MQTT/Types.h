/*
 * Types.h
 *
 *  Created on: Nov 28, 2014
 *      Author: dron
 */

#ifndef MQTTTYPES_H_
#define MQTTTYPES_H_

#include <Poco/Delegate.h>
#include <Poco/BasicEvent.h>
#include <string>
#include <vector>

namespace TBS {
	namespace MQTT {

		class Topic {
			public:
				Topic();
				Topic(const std::string & topic);
				~Topic();

				std::string topic() const;
				std::string part(int index) const;
				std::size_t size() const;

			private:
				std::string topic_;
				typedef std::vector<std::string> Parts;
				Parts parts_;
		};

		enum ConnectStatus {
			ConnectOk, ConnectFailed
		};

		struct QoS {
			enum QoSType {
				NoConfirmation = 0,
				Deliver = 1,
				DeliverOnce = 2,
			};
		};

		struct Message {
				int mid;
				Topic topic;
				std::string payload;
				QoS::QoSType qos;
				bool retain;

				Message(int mid, const Topic & topic, const std::string & payload, const QoS::QoSType & qos, bool retain);
		};

		struct Subscribe {
				int mid;
				int qosCount;
				int grantedQos;
		};

		typedef Poco::BasicEvent<ConnectStatus> ConnectEvent;
		typedef Poco::BasicEvent<Message> MessageEvent;
		typedef Poco::BasicEvent<Subscribe> SubscribeEvent;


	} /* namespace MQTT */
} /* namespace TBS */

#endif /* TYPES_H_ */
