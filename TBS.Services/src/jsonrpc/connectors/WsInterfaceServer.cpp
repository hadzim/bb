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


	static Poco::Net::HTTPRequestHandlerFactory::Ptr createWsJsonRPC(ServiceHandlers::Ptr h, const TBS::Services::JsonServerParams & p){
		return new RootHandlerFactory<WsRootHandler>(h, p);
	}

	WsInterfaceServer::WsInterfaceServer(const TBS::Services::JsonServerParams & p) :
		SharedHttpInterfaceServer(p, createWsJsonRPC){

	}


} /* namespace jsonrpc */

#endif
