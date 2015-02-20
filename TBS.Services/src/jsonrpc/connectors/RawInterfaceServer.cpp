/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    httpserver.cpp
 * @date    31.12.2012
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "jsonrpc/connectors/RawInterfaceServer.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>

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
#include <Poco/Net/TCPServer.h>
#include <Poco/Net/TCPServerConnection.h>

namespace jsonrpc {

	class ServiceConnection: public Poco::Net::TCPServerConnection {
		public:
			ServiceConnection(const Poco::Net::StreamSocket& s, ServiceHandlers::WeakPtr serviceHandlers, TBS::Services::JsonServerParams & p) :
					Poco::Net::TCPServerConnection(s), handlerProvider(serviceHandlers), p(p) {
			}

			void run() {
				Poco::Net::StreamSocket& ss = socket();
				try {
					char buffer[4096];
					int n = ss.receiveBytes(buffer, sizeof(buffer));
					while (n > 0) {
						std::string requestString(buffer, n);
						LTRACE("json") << "retrieved: " << requestString << LE;
						std::string::size_type question = requestString.find('\n');
						if (question == std::string::npos) {
							LTRACE("json") << "wrong packet " << LE;
							throw Poco::Exception("Wrong packet retrieved");
						}

						std::string serviceName = requestString.substr(0, question);
						std::string request = requestString.substr(question+1);

						LTRACE("json") << "serviceName: " << serviceName << LE;
						LTRACE("json") << "request: " << request << LE;

						std::string responseString;

						LTRACE("json") << "request accepted" << LE;

						if (auto sHandlerProvider = handlerProvider.lock()) {
							sHandlerProvider->getHandler(serviceName)->HandleRequest(request, responseString);
						} else {
							throw Poco::Exception("Request refused");
						}


						LTRACE("json") << "response generated" << LE;

						ss.sendBytes(responseString.data(), responseString.length());

						n = ss.receiveBytes(buffer, sizeof(buffer));
					}
				} catch (Poco::Exception& exc) {
					std::cerr << "EchoConnection: " << exc.displayText() << std::endl;
				}
			}
		private:
			ServiceHandlers::WeakPtr handlerProvider;
			TBS::Services::JsonServerParams p;
	};


	class RawHandlerFactory: public Poco::Net::TCPServerConnectionFactory {
		public:
			RawHandlerFactory(ServiceHandlers::WeakPtr handlerProvider, const TBS::Services::JsonServerParams & p) :
					handlerProvider(handlerProvider), p(p) {
			}
			virtual ~RawHandlerFactory() {
			}

			virtual Poco::Net::TCPServerConnection* createConnection(const Poco::Net::StreamSocket& socket){
				return new ServiceConnection(socket, handlerProvider, p);
			}

		private:
			ServiceHandlers::WeakPtr handlerProvider;
			TBS::Services::JsonServerParams p;
	};


	RawInterfaceServer::RawInterfaceServer(const TBS::Services::JsonServerParams & p) :
			p(p) {

		// set-up a server socket
		Poco::Net::ServerSocket svs(p.port());
		// set-up a TCPServer instance
		srv = std::unique_ptr<Poco::Net::TCPServer>(new Poco::Net::TCPServer(new RawHandlerFactory(this->handlers(), p), svs));
	}

	RawInterfaceServer::~RawInterfaceServer() {
		srv->stop();
	}

	bool RawInterfaceServer::StartListening() {
		std::cout << "multi server start listening " << std::endl;
		srv->start();
		return true;
	}

	bool RawInterfaceServer::StopListening() {
		std::cout << "multi server stop listening " << std::endl;
		srv->stop();
		return true;

	}


}
/* namespace jsonrpc */

