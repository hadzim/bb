/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    client.h
 * @date    03.01.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef CLIENT_H_
#define CLIENT_H_

#include "clientconnector.h"
#include "rpcprotocolclient.h"
#include "json/json.h"
#include "TBS/Services/Types.h"

#include <vector>
#include <map>

namespace jsonrpc {
	class GEN_SERVICE_API Client {
		public:
			Client(AbstractClientConnector::Ptr connector);
			~Client();

			void CallMethod(const std::string &name, const Json::Value &paramter, Json::Value& result);
			Json::Value CallMethod(const std::string& name, const Json::Value& paramter);
			void CallNotification(const std::string& name, const Json::Value& paramter);

		private:
			AbstractClientConnector::Ptr connector;
			RpcProtocolClient protocol;

	};

} /* namespace jsonrpc */
#endif /* CLIENT_H_ */
