/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    httpserver.cpp
 * @date    31.12.2012
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "jsonrpc/connectors/WsInterfaceServer.h"

#ifndef OLDPOCO13

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/SecureServerSocket.h"
#include "Poco/Net/HTTPBasicCredentials.h"

#include "Poco/Net/HTTPServer.h"


#include <Poco/StreamCopier.h>
#include "jsonrpc/ServiceHandlers.h"

#include "Poco/MD5Engine.h"
#include "Poco/DigestStream.h"
#include "Poco/DigestEngine.h"

#include "Poco/File.h"
#include <fstream>

#include "HttpServerHelpers.h"
#include <Poco/Net/NetException.h>

#include "TBS/Log.h"

namespace jsonrpc {

	class WsRootHandler: public Poco::Net::HTTPRequestHandler {
		public:

			WsRootHandler(ServiceHandlers::WeakPtr serviceHandlers, TBS::Services::JsonServerParams & p) :
					handlerProvider(serviceHandlers), p(p) {
			}

			void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {

				LTRACE("json") << "handle request" << LE;


				try {
					WebSocketWriter::WSStrongPtr ws(new Poco::Net::WebSocket(request, response));
					char buffer[4096];
					int flags;
					int n;
					do {

						std::string serviceName = request.getURI();
						if (serviceName.find('/') == 0) {
							serviceName = serviceName.substr(1);
						}
						std::string::size_type question = serviceName.find('?');
						if (question != std::string::npos) {
							serviceName = serviceName.substr(0, question - 1);
						}

						n = ws->receiveFrame(buffer, sizeof(buffer), flags);

						if (flags & Poco::Net::WebSocket::FRAME_OP_TEXT){
							std::string requestString(buffer, n);
							std::string responseString;

							LTRACE("json") << "request accepted" << LE;

							if (auto sHandlerProvider = handlerProvider.lock()) {
								sHandlerProvider->getHandler(serviceName)->HandleRequest(requestString, responseString);
							} else {
								throw Poco::Exception("Request refused");
							}


							LTRACE("json") << "response generated" << LE;

							ws->sendFrame(responseString.data(), responseString.length(), flags);

							LTRACE("json") << "response sent" << LE;
						}
					} while (n > 0 || (flags & Poco::Net::WebSocket::FRAME_OP_BITMASK) != Poco::Net::WebSocket::FRAME_OP_CLOSE);

				} catch (Poco::Net::WebSocketException& exc) {
					switch (exc.code()) {
						case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
							response.set("Sec-WebSocket-Version", Poco::Net::WebSocket::WEBSOCKET_VERSION);
							// fallthrough
						case Poco::Net::WebSocket::WS_ERR_NO_HANDSHAKE:
						case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
						case Poco::Net::WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
							response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
							response.setContentLength(0);
							response.send();
							break;
					}
				}
				LTRACE("json") << "handle request done" << LE;
			}
		private:
			ServiceHandlers::WeakPtr handlerProvider;
			TBS::Services::JsonServerParams p;
	};



	WsInterfaceServer::WsInterfaceServer(const TBS::Services::JsonServerParams & p) :
			p(p) {

		Poco::Net::HTTPServerParams* pParams = new Poco::Net::HTTPServerParams;
		pParams->setMaxQueued(100);
		pParams->setMaxThreads(2);
		if (p.isHttps()) {
			std::cout << "service via https: PK: " << p.getHttpsPrivateKey() << " CT:" << p.getHttpsCertificate() << std::endl;
			Poco::Net::Context::Ptr context = new Poco::Net::Context(Poco::Net::Context::SERVER_USE, p.getHttpsPrivateKey(), p.getHttpsCertificate(), "");
			// disable session cache because of Firefox (less memory consuming than session cache enabling)
#ifndef OLDPOCO13
			context->disableStatelessSessionResumption();
#endif
			Poco::Net::SecureServerSocket svs(p.port(), 64, context);
			srv = std::unique_ptr < Poco::Net::HTTPServer > (new Poco::Net::HTTPServer(new RootHandlerFactory<WsRootHandler>(this->handlers(), p), svs, pParams));
			std::cout << "multi https server listens on " << srv->port() << std::endl;
		} else {
			Poco::Net::ServerSocket svs(p.port()); // set-up a server socket
			srv = std::unique_ptr < Poco::Net::HTTPServer > (new Poco::Net::HTTPServer(new RootHandlerFactory<WsRootHandler>(this->handlers(), p), svs, pParams));
			std::cout << "multi server listens on " << srv->port() << std::endl;
		}
	}

	WsInterfaceServer::~WsInterfaceServer() {
		srv->stop();
	}

	bool WsInterfaceServer::StartListening() {
		std::cout << "multi server start listening " << std::endl;
		srv->start();
		return true;
	}

	bool WsInterfaceServer::StopListening() {
		std::cout << "multi server stop listening " << std::endl;
		srv->stop();
		return true;

	}


} /* namespace jsonrpc */

#endif
