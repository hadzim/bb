/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    httpclient.cpp
 * @date    02.01.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "jsonrpc/connectors/SerialInterfaceClient.h"
#include <string>
#include <string.h>
#include <cstdlib>

#include <iostream>

#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/StreamCopier.h>
#include <Poco/Thread.h>
#include "Poco/Net/HTTPBasicCredentials.h"
#include <Poco/Net/HTTPSClientSession.h>
#include "TBS/Log.h"

#include "SerialLayerHelpers.h"



using namespace std;
using namespace Poco::Net;
using namespace Poco;
namespace jsonrpc {


	SerialInterfaceClient::SerialInterfaceClient(const  std::string & interfaceName, const TBS::Services::SerialParams & p) :
			interfaceName(interfaceName), p(p) {


		layer = new TBS::Services::SerialTransportLayer(p.getSerialLayer());
		client = TBS::Services::createClient(layer, p.getTransportLayer());
	}

	SerialInterfaceClient::~SerialInterfaceClient() {
		LTRACE("json") << "destruct SERIAL LINE conector " << LE;
	}

	void SerialInterfaceClient::SendMessage(const std::string& message, std::string& result) {
		result = client->sendRequest(message, this->interfaceName);
	}

} /* namespace jsonrpc */
