/*
 * Types.cpp
 *
 *  Created on: Nov 28, 2014
 *      Author: dron
 */

#include "MQTT/Types.h"

#include <Poco/StringTokenizer.h>
#include <sstream>

namespace TBS {
	namespace MQTT {

		Topic::Topic(){}

		Topic::Topic(const std::string & topic) :
				topic_(topic) {
			Poco::StringTokenizer t(topic_, "/");
			parts_.assign(t.begin(), t.end());
		}

		Topic::~Topic() {

		}

		std::string Topic::topic() const {
			return topic_;
		}

		std::string Topic::part(int index) const {
			try {
				return parts_.at(index);
			} catch (std::exception & e) {
				std::stringstream s;
				s << "get topic part " << index << " not available (topic: " << topic_ << ")";
				throw Poco::Exception(s.str());
			}
		}

		std::size_t Topic::size() const {
			return this->parts_.size();
		}

		Message::Message(int mid, const Topic & topic, const std::string & payload, const QoS::QoSType & qos, bool retain) :
			mid(mid),
			topic(topic),
			payload(payload),
			qos(qos),
			retain(retain){

		}

	} /* namespace MQTT */
} /* namespace TBS */
