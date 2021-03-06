/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    client.cpp
 * @date    03.01.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "jsonrpc/client.h"
#include "jsonrpc/rpcprotocolclient.h"
#include "jsonrpc/exception.h"
#include <Poco/Exception.h>
#include "TBS/Log.h"

namespace jsonrpc {
	Client::Client(AbstractClientConnector::Ptr connector) :
			connector(connector) {
	}

	Client::~Client() {

	}

	void Client::CallMethod(const std::string &name, const Json::Value &paramter, Json::Value& result) {
		LTRACE("json") << "call method: " << name << LE;
		std::string request, response;
		protocol.BuildRequest(name, paramter, request, false);
		LTRACE("json") << "request: " << request << LE;
		connector->SendMessage(request, response);
		protocol.HandleResponse(response, result);
		LTRACE("json") << "done" << LE;

	}

	Json::Value Client::CallMethod(const std::string& name, const Json::Value& parameter) {
		Json::Value result;
		this->CallMethod(name, parameter, result);
		return result;
	}

	void Client::CallNotification(const std::string& name, const Json::Value& parameter) {
		std::string request, response;
		protocol.BuildRequest(name, parameter, request, true);
		connector->SendMessage(request, response);
	}

} /* namespace jsonrpc */
