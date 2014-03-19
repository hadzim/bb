/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    httpserver.cpp
 * @date    31.12.2012
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "jsonrpc/connectors/httpinterfaceserver.h"
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

namespace jsonrpc {

	std::string md5Hash(std::string val) {
		Poco::MD5Engine md5;
		Poco::DigestOutputStream ostr(md5);
		ostr << val;
		ostr.flush(); // Ensure everything gets passed to the digest engine
		const Poco::DigestEngine::Digest& digest = md5.digest(); // obtain result
		std::string result = Poco::DigestEngine::digestToHex(digest);
		return result;
	}

	class MultiRootHandler: public Poco::Net::HTTPRequestHandler {
		public:

			MultiRootHandler(ServiceHandlers & serviceHandlers, HttpServerParams p) :
					handlerProvider(serviceHandlers), p(p) {
			}

			void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
				std::string errorMessage = "";
				try {

					std::string serviceName = request.getURI();

					bool isRoot = serviceName == "/";
/*
					std::cout << "SERVER service name: " << serviceName << std::endl;
					std::cout << "SERVER request length: " << request.getContentLength() << std::endl;
					std::cout << "SERVER request method: " << request.getMethod() << std::endl;
					std::cout << "SERVER request chunked: " << request.getChunkedTransferEncoding() << std::endl;
					std::cout << "SERVER request type: " << request.getContentType() << std::endl;
					std::cout << "SERVER request host: " << request.getHost() << std::endl;
*/
					if (p.allowCrossDomain && !isRoot) {
						try {
							response.set("Access-Control-Allow-Origin", request.get("Origin"));
						} catch (Poco::Exception & e){
							response.set("Access-Control-Allow-Origin", "*");
						}
						response.set("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
						response.set("Access-Control-Max-Age", "1000");
						response.set("Access-Control-Allow-Headers", "origin, x-csrftoken, content-type, accept, authorization");
					}
					if (request.getMethod() == "OPTIONS"){
						//std::cout << "options to be done" << std::endl;
						response.send();
						//std::cout << "options done" << std::endl;
						return;
					}

					if (p.isProtected) {
						//std::cout << "protected" << std::endl;
						if (!request.hasCredentials()) {
							std::cout << "protected - no credentials" << std::endl;
							response.requireAuthentication("TBS Service Authentication");
							response.setContentLength(0);
							response.send();
							return;
						} else {
							//std::cout << "protected - credentials" << std::endl;
							Poco::Net::HTTPBasicCredentials cred(request);
							std::string user = cred.getUsername();
							std::string pwd = cred.getPassword();

							std::string userTarget = p.username;
							std::string pwdTarget = p.passwordMd5Hash;

							std::cout << "protected - match" << user << ":" << userTarget << " and apss " << md5Hash(pwd) << " vs " << pwdTarget  << std::endl;

							if (user != userTarget || md5Hash(pwd) != pwdTarget) {
								std::cout << "protected - bad credentials" << std::endl;

								response.requireAuthentication("TBS Service Authentication");
								response.setContentLength(0);
								response.send();
								return;
							}
						}

					} else {
						//std::cout << "not protected" << std::endl;
					}

					if (serviceName.find('/') == 0) {
						serviceName = serviceName.substr(1);
					}
					std::string::size_type question = serviceName.find('?');
					if (question != std::string::npos) {
						serviceName = serviceName.substr(0, question - 1);
					}

					if (isRoot && p.isDocFile){
						std::cout << "docu " << p.docFile << std::endl;
						Poco::File f(p.docFile);
						if (!f.exists()){
							throw Poco::Exception("No root doc data");
						}
						std::cout << "docu real " << p.docFile << std::endl;
						response.setContentType("text/html");
						response.setChunkedTransferEncoding(true);
						std::ostream & o = response.send();
						std::ifstream docf(p.docFile.c_str());
						Poco::StreamCopier::copyStream(docf, o);
						return;
					}

					std::cout << "service name: " << serviceName << std::endl;

					std::istream& rs = request.stream();
					std::stringstream outstr;
					Poco::StreamCopier::copyStream(rs, outstr);
					std::string rsp;
					std::string req = outstr.str();

					std::cout << "requset: " << req << std::endl;

					//this->handlerProvider.GetHandler()

					this->handlerProvider.getHandler(serviceName)->HandleRequest(req, rsp);

					std::cout << "response: " << rsp << std::endl;

					response.setContentType("application/json");
					response.sendBuffer(rsp.data(), rsp.length());
				} catch (Poco::Exception & e){
					errorMessage = e.message();
					std::cout << "EXception: " << e.displayText() << std::endl;
				} catch (std::exception & e){
					std::cout << "EXception: " << e.what() << std::endl;
					errorMessage = e.what();
				} catch (...){
					std::cout << "EXception: ??" << std::endl;
					errorMessage = "Unknown error";
				}



			}
		private:
			ServiceHandlers & handlerProvider;
			HttpServerParams p;
	};

	class MultiRequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory {
		public:
			MultiRequestHandlerFactory(ServiceHandlers & handlerProvider, const HttpServerParams & p) :
					handlerProvider(handlerProvider), p(p) {
				//std::cout < "hfactory " << &handlerProvider << std::endl;
			}
			virtual ~MultiRequestHandlerFactory() {
			}
			Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) {
				return new MultiRootHandler(handlerProvider, p);
			}
		private:
			ServiceHandlers & handlerProvider;
			HttpServerParams p;
	};

	HttpInterfaceServer::HttpInterfaceServer(const HttpServerParams & p) :
			p(p) {

		Poco::Net::HTTPServerParams* pParams = new Poco::Net::HTTPServerParams;
		pParams->setMaxQueued(100);
		pParams->setMaxThreads(2);
		if (p.isHttps) {
			std::cout << "service via https: PK: " << p.privateKey << " CT:" << p.certificate << std::endl;
			Poco::Net::Context::Ptr context = new Poco::Net::Context(Poco::Net::Context::SERVER_USE, p.privateKey, p.certificate, "");
			// disable session cache because of Firefox (less memory consuming than session cache enabling)
			context->disableStatelessSessionResumption();
			Poco::Net::SecureServerSocket svs(p.port, 64, context);
			srv = std::auto_ptr<Poco::Net::HTTPServer>(new Poco::Net::HTTPServer(new MultiRequestHandlerFactory(handlers_, p), svs, pParams));
			std::cout << "multi https server listens on " << srv->port() << std::endl;
		} else {
			Poco::Net::ServerSocket svs(p.port); // set-up a server socket
			srv = std::auto_ptr<Poco::Net::HTTPServer>(new Poco::Net::HTTPServer(new MultiRequestHandlerFactory(handlers_, p), svs, pParams));
			std::cout << "multi server listens on " << srv->port() << std::endl;
		}
	}

	HttpInterfaceServer::~HttpInterfaceServer() {
		srv->stop();
	}

	bool HttpInterfaceServer::StartListening() {
		std::cout << "multi server start listening " << std::endl;
		srv->start();
		return true;
	}

	bool HttpInterfaceServer::StopListening() {
		std::cout << "multi server stop listening " << std::endl;
		srv->stop();
		return true;

	}
	ServiceHandlers & HttpInterfaceServer::handlers() {
		return this->handlers_;
	}

} /* namespace jsonrpc */
