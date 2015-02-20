/*
 * JsonServices.h
 *
 *  Created on: Oct 7, 2013
 *      Author: root
 */

#ifndef SERIALJSONSERVICES_H_
#define SERIALJSONSERVICES_H_

#include <string>

#include "TBS/Nullable.h"

namespace TBS {
	namespace Services {

		class TransportLayerParams {
			public:

				enum Type {
					Plain, Secured
				};

				//creates plain transport layer
				TransportLayerParams() :
						type(Plain) {

				}
				TransportLayerParams(std::string masterKey, std::string id) :
						type(Secured) {
					this->masterKey.set(masterKey);
					this->id.set(id);
				}

			public:
				Type getType() const {
					return this->type;
				}

				std::string getMasterKey() const {
					return this->masterKey.val();
				}

				std::string getId() const {
					return this->id.val();
				}

			private:
				Type type;
				TBS::Nullable<std::string> masterKey;
				TBS::Nullable<std::string> id;
		};

		class SerialLayerParams {
			public:

				SerialLayerParams(const std::string &port = "", uint32_t baudrate = 9600, int timeoutMs = 300, int sleepTimeMs = 5) :
						port(port), baudrate(baudrate), timeoutMs(timeoutMs), sleepTimeMs(sleepTimeMs) {

				}
			public:
				std::string getPort() const {
					return this->port;
				}
				uint32_t getBaudrate() const {
					return this->baudrate;
				}

				int getTimeoutMs() const {
					return this->timeoutMs;
				}

				int getSleepTimeMs() const {
					return this->sleepTimeMs;
				}
			private:
				std::string port;
				uint32_t baudrate;
				int timeoutMs;
				int sleepTimeMs;
		};

		class SerialParams {
			public:

				SerialParams(const SerialLayerParams & serialParams, const TransportLayerParams & transportParams = TransportLayerParams()) :
						serialParams(serialParams), transportParams(transportParams) {

				}
			public:
				const SerialLayerParams & getSerialLayer() const {
					return this->serialParams;
				}
				const TransportLayerParams & getTransportLayer() const {
					return this->transportParams;
				}

			private:
				SerialLayerParams serialParams;
				TransportLayerParams transportParams;
		};
	}
}
#endif /* SERIALJSONSERVICES_H_ */
