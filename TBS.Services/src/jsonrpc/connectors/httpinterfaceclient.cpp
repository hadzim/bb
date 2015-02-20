/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    httpclient.cpp
 * @date    02.01.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "jsonrpc/connectors/httpinterfaceclient.h"
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


	HttpInterfaceClient::HttpInterfaceClient(const std::string& ifacename, const TBS::Services::JsonClientParams & p) :
			interfaceName(ifacename), p(p) {

		LTRACE("Json") << "create json http conector " << p.hostName() << " port " << p.port() << LE;

		if (p.isHttps()){
			//std::cout << "no verify " << std::endl;
			Poco::Net::Context::Ptr context = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE);

			//std::cout << "create https for "<< p.hostName() << " port " << p.port() << std::endl;
			client = ClientPtr(new Poco::Net::HTTPSClientSession(p.hostName(), p.port(), context));
			//std::cout << "create https done" << std::endl;
		} else {
			//std::cout << "create http" << std::endl;
			client = ClientPtr(new Poco::Net::HTTPClientSession(p.hostName(), p.port()));
			//std::cout << "create http done" << std::endl;
		}

		client->setTimeout(5l * 1000000l);
	}

	HttpInterfaceClient::~HttpInterfaceClient() {
		LTRACE("Json") << "destruct http conector " << LE;
	}

	void HttpInterfaceClient::SendMessage(const std::string& message, std::string& result) {

		LTRACE("Json") << "send message via http conector " << LE;

		Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, "/" + interfaceName);
		request.setContentType("application/json");
		request.set("charsets:", "utf-8");

		if (p.isProtected()){
			Poco::Net::HTTPBasicCredentials c(p.getUserName(), p.getPassword());
			//TODO on win !!!
#ifndef _WIN32
			c.authenticate(request);
#endif
		}

		request.setContentLength((int) message.length());
		client->sendRequest(request) << message;

		//std::cout << "send " << message << std::endl;

		Poco::Net::HTTPResponse response;
		std::istream& rs = client->receiveResponse(response);
		std::stringstream outstr;
		Poco::StreamCopier::copyStream(rs, outstr);

		result = outstr.str();

		//std::cout << "res " << result << std::endl;

		LTRACE("Json") << "send message via http conector done" << LE;

	}

} /* namespace jsonrpc */
