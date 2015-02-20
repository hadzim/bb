/*
 * TransportLayer.h
 *
 *  Created on: Nov 6, 2014
 *      Author: dron
 */

#ifndef ITRANSPORTLAYER_H_
#define ITRANSPORTLAYER_H_


#include <Poco/BasicEvent.h>
#include "TBS/Nullable.h"

#include <Poco/UUIDGenerator.h>

namespace TBS {
	namespace Services {

		class ITransportLayer {
			public:

				typedef Poco::SharedPtr<ITransportLayer> Ptr;

				virtual ~ITransportLayer() {

				}

				virtual void write(const std::string &) = 0;
				virtual std::string read() = 0;
		};

		class ITransportClient {
			public:

				typedef Poco::SharedPtr<ITransportClient> Ptr;

				virtual ~ITransportClient() {
				}

				virtual std::string sendRequest(const std::string & request, const std::string interface) = 0;
		};

		class ITransportServer {
			public:

				typedef Poco::SharedPtr<ITransportServer> Ptr;

				virtual ~ITransportServer() {
				}

				virtual void loop() = 0;

				struct Message {
						std::string request;
						std::string interface;
						std::string response;
				};

				Poco::BasicEvent<Message> OnMessage;
		};



	} /* namespace Services */
} /* namespace TBS */

#endif /* TRANSPORTLAYER_H_ */
