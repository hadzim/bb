/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    httpserver.cpp
 * @date    31.12.2012
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "jsonrpc/connectors/httpserver.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include "Poco/Net/HTTPServer.h"
#include <Poco/StreamCopier.h>

namespace jsonrpc {

	class RootHandler: public Poco::Net::HTTPRequestHandler {
		public:

			RootHandler(ServerHandlerProvider & handlerProvider) :
					handlerProvider(handlerProvider) {
			}

			void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {

				std::istream& rs = request.stream();
				std::stringstream outstr;
				Poco::StreamCopier::copyStream(rs, outstr);
				std::string rsp;
				std::string req = outstr.str();
				std::cout << "req" << req << std::endl;

				this->handlerProvider.GetHandler().HandleRequest(req, rsp);

				response.setChunkedTransferEncoding(true);
				response.setContentType("application/json");
				std::ostream& ostr = response.send();
				ostr << rsp;
			}
		private:
			ServerHandlerProvider & handlerProvider;
	};

	class MyRequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory {
		public:
			MyRequestHandlerFactory(ServerHandlerProvider & handlerProvider) :
					handlerProvider(handlerProvider) {
			}
			virtual ~MyRequestHandlerFactory(){}
			Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) {
				return new RootHandler(handlerProvider);
			}
		private:
			ServerHandlerProvider & handlerProvider;
	};

	HttpServer::HttpServer(int port, bool enableSpecification, const std::string &sslcert) {

		Poco::Net::HTTPServerParams* pParams = new Poco::Net::HTTPServerParams;
		pParams->setMaxQueued(100);
		pParams->setMaxThreads(2);
		Poco::Net::ServerSocket svs(port); // set-up a server socket
		srv = std::auto_ptr < Poco::Net::HTTPServer > (new Poco::Net::HTTPServer(new MyRequestHandlerFactory(*this), svs, pParams));
		std::cout << "server listens on " << srv->port() << std::endl;
	}

	HttpServer::~HttpServer() {
		srv->stop();
	}

	bool HttpServer::StartListening() {
		srv->start();
		return true;
	}

	bool HttpServer::StopListening() {
		srv->stop();
		return true;

	}


} /* namespace jsonrpc */
