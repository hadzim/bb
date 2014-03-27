/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    httpclient.cpp
 * @date    02.01.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "jsonrpc/connectors/httpclient.h"
#include <string>
#include <string.h>
#include <cstdlib>

#include <iostream>

#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/StreamCopier.h>

using namespace std;
using namespace Poco::Net;
using namespace Poco;
namespace jsonrpc {


	HttpClient::HttpClient(const std::string& host, int port) :
			AbstractClientConnector(), url(host), port(port), client(host, port) {

	}

	HttpClient::~HttpClient() {

	}

	void HttpClient::SendMessage(const std::string& message, std::string& result){

		Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, "/");

		request.setContentType("application/json");
		request.set("charsets:", "utf-8");
		request.setChunkedTransferEncoding(true);

		std::ostream & o = client.sendRequest(request);
		o << message;

		std::cout << "senda " << message << std::endl;


		Poco::Net::HTTPResponse response;
		std::istream& rs = client.receiveResponse(response);
		std::stringstream outstr;
		Poco::StreamCopier::copyStream(rs, outstr);

		result = outstr.str();

		std::cout << "res " << result << std::endl;


		//TODO maybe throw JsonRpcException(Errors::ERROR_CLIENT_CONNECTOR, str.str());
	}

	void HttpClient::SetUrl(const std::string& url) {
		this->url = url;
		throw Poco::NotImplementedException("cannot set url");
	}

} /* namespace jsonrpc */
