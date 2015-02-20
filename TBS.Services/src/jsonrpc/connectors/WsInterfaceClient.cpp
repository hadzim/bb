/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    httpclient.cpp
 * @date    02.01.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "jsonrpc/connectors/WsInterfaceClient.h"


#ifndef OLDPOCO13

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

using namespace std;
using namespace Poco::Net;
using namespace Poco;
namespace jsonrpc {


	WsInterfaceClient::WsInterfaceClient(const std::string& ifacename, const TBS::Services::JsonClientParams & p) :
			interfaceName(ifacename), p(p) {

		if (p.isHttps()){
			std::cout << "no verify " << std::endl;
			Poco::Net::Context::Ptr context = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE);

			std::cout << "create https for "<< p.hostName() << " port " << p.port() << std::endl;
			client = ClientPtr(new Poco::Net::HTTPSClientSession(p.hostName(), p.port(), context));
			std::cout << "create https done" << std::endl;
		} else {
			std::cout << "create http" << std::endl;
			client = ClientPtr(new Poco::Net::HTTPClientSession(p.hostName(), p.port()));
			std::cout << "create http done" << std::endl;
		}

		//client->setTimeout(5l * 1000000l);

		request = RequestPtr(new Poco::Net::HTTPRequest(Poco::Net::HTTPRequest::HTTP_GET, "/" + interfaceName));
		request->setContentType("application/json");
		request->set("charsets:", "utf-8");

		if (p.isProtected()){
			Poco::Net::HTTPBasicCredentials c(p.getUserName(), p.getPassword());
			//TODO on win !!!
#ifndef _WIN32
			c.authenticate(*request);
#endif
		}

		this->ws = WsPtr(new Poco::Net::WebSocket(*client, *request, response));
		LTRACE("json") << "interface client constructed"  << LE;
	}

	WsInterfaceClient::~WsInterfaceClient() {
		LTRACE("json") << "interface client destruct"  << LE;

		ws->shutdown();
		LTRACE("json") << "interface client destruct after shutdown"  << LE;

		//int flags;
		//char buffer[1500];
		//int n = ws->receiveFrame(buffer, sizeof(buffer), flags);
		//if (!((flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) == Poco::Net::WebSocket::FRAME_OP_CLOSE)){
		//	LWARNING("WSClient") << "Closed with inproper flags" << LE;
		//}
		request.reset();

		LTRACE("json") << "interface client destructed"  << LE;
	}

	void WsInterfaceClient::SendMessage(const std::string& message, std::string& result) {

		LTRACE("json") << "message to be send" << LE;

		int flags = Poco::Net::WebSocket::FRAME_TEXT;
		ws->sendFrame(message.data(), message.length(), flags);
		LTRACE("json") << "sent" << LE;
		char buffer[4096];
		int n = ws->receiveFrame(buffer, sizeof(buffer), flags);

		LTRACE("json") << "received: " << n << LE;

		std::string responseString(buffer, n);
		result = responseString;

		LTRACE("json") << "done: result is " << result  << LE;
	}
} /* namespace jsonrpc */

#endif
