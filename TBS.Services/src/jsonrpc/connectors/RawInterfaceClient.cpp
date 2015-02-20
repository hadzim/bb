/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    httpclient.cpp
 * @date    02.01.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "jsonrpc/connectors/RawInterfaceClient.h"
#include <string>
#include <string.h>
#include <cstdlib>

#include <iostream>

#include <Poco/StreamCopier.h>
#include <Poco/Thread.h>
#include "TBS/Log.h"

using namespace std;
using namespace Poco::Net;
using namespace Poco;
namespace jsonrpc {


	RawInterfaceClient::RawInterfaceClient(const std::string& ifacename, const TBS::Services::JsonClientParams & p) :
			interfaceName(ifacename), p(p) {

		Poco::Net::SocketAddress sa(p.hostName(), p.port());

		socket = SocketPtr(new StreamSocket(sa));

		LTRACE("json") << "interface client constructed"  << LE;
	}

	RawInterfaceClient::~RawInterfaceClient() {

		//int flags;
		//char buffer[1500];
		//int n = ws->receiveFrame(buffer, sizeof(buffer), flags);
		//if (!((flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) == Poco::Net::WebSocket::FRAME_OP_CLOSE)){
		//	LWARNING("WSClient") << "Closed with inproper flags" << LE;
		//}

		LTRACE("json") << "interface client destructed"  << LE;
	}

	void RawInterfaceClient::SendMessage(const std::string& message, std::string& result) {

		LTRACE("json") << "message to be send" << LE;

		std::string final = interfaceName + "\n" + message;

		socket->sendBytes(final.data(), final.length());
		LTRACE("json") << "sent " << final << LE;
		char buffer[4096];
		int n = socket->receiveBytes(buffer, sizeof(buffer));
		LTRACE("json") << "received: " << n << LE;

		std::string responseString(buffer, n);
		result = responseString;

		LTRACE("json") << "done: result is " << result  << LE;
	}
} /* namespace jsonrpc */
