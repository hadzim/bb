#include <jsonrpc/connectors/RawInterfaceClient.h>
#include <jsonrpc/connectors/SerialInterfaceClient.h>
#include "TBS/Services/Json/JsonServicesImpl.h"
/*
 * JsonServerCommChannelHolder.cpp
 *
 *  Created on: Oct 7, 2013
 *      Author: root
 */

namespace TBS {
	namespace Services {

		jsonrpc::AbstractClientConnector::Ptr createClientConnector(const std::string & name, const JsonClientParams & params) {
			if (params.protocol() == JsonClientParams::JsonHttp) {
				return new jsonrpc::HttpInterfaceClient(name, params);
			} else if (params.protocol() == JsonClientParams::JsonWs) {
				return new jsonrpc::WsInterfaceClient(name, params);
			} else if (params.protocol() == JsonClientParams::JsonRaw) {
				return new jsonrpc::RawInterfaceClient(name, params);
			} else {
				throw Poco::Exception("Cannot create client - undefined protocol");
			}
		}

		jsonrpc::AbstractClientConnector::Ptr createClientConnector(const std::string & name, const SerialParams & params) {
			return new jsonrpc::SerialInterfaceClient(name, params);
		}

	} /* namespace Services */
} /* namespace TBS */
