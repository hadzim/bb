/*
 * TransportLayer.cpp
 *
 *  Created on: Nov 6, 2014
 *      Author: dron
 */

#include "TransportLayer.h"

namespace TBS {
	namespace Services {

		ITransportClient::Ptr createClient(ITransportLayer::Ptr t, const TransportLayerParams & params) {
					if (params.getType() == TransportLayerParams::Plain) {
						return new PlainClient(t);
					}
					if (params.getType() == TransportLayerParams::Secured) {
						SecureClient::SecureParams p;
						p.id = params.getId();
						p.masterKey = params.getMasterKey();
						return new SecureClient(t, p);
					}
					throw Poco::Exception("Unknown transport client type");
				}

				ITransportServer::Ptr createServer(ITransportLayer::Ptr t, const TransportLayerParams & params) {
					if (params.getType() == TransportLayerParams::Plain) {
						return new PlainServer(t);
					}
					if (params.getType() == TransportLayerParams::Secured) {
						SecureServer::SecureParams p;
						p.masterKey = params.getMasterKey();
						return new SecureServer(t, p);
					}
					throw Poco::Exception("Unknown transport server type");
				}

	} /* namespace Services */
} /* namespace TBS */
