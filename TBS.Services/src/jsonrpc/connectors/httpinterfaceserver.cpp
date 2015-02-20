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

#include "HttpServerHelpers.h"
#include "TBS/Log.h"

namespace jsonrpc {



	class MultiRootHandler: public Poco::Net::HTTPRequestHandler {
		public:

			MultiRootHandler(ServiceHandlers::WeakPtr serviceHandlers, TBS::Services::JsonServerParams & p) :
					handlerProvider(serviceHandlers), p(p) {
				//LERROR("Service.Jsonp") << "root holder construct: obj" << (int)this << " ref: " << (int)&this->handlerProvider << LE;
			}
			~MultiRootHandler(){
				//LERROR("Service.Jsonp") << "root holder destruct: obj" << (int)this << " ref: " << (int)&this->handlerProvider << LE;
			}

			void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
				std::string errorMessage = "";
				try {

					HttpServerHelpers::ReturnType ret = HttpServerHelpers::preprocess(p, request, response);
					if (ret == HttpServerHelpers::RequestFinished){
						return;
					}

					std::string serviceName = request.getURI();

					std::cout << "servicename before: " << serviceName << std::endl;

					if (serviceName.find('/') == 0) {
						serviceName = serviceName.substr(1);
					}
					std::string::size_type question = serviceName.find('?');
					if (question != std::string::npos) {
						serviceName = serviceName.substr(0, question);
					}

					std::cout << "servicename after: " << serviceName << std::endl;

					for (auto i = p.getRequestHandlers().begin(); i != p.getRequestHandlers().end(); i++){
						if ((*i)->canHandle(serviceName)){
							std::cout << "SPECIAL HANDLING of " << serviceName << std::endl;
							(*i)->handle(request, response);
							return;
						}
					}

					//std::cout << "service name: " << serviceName << std::endl;

					std::istream& rs = request.stream();
					std::stringstream outstr;
					Poco::StreamCopier::copyStream(rs, outstr);
					std::string rsp;
					std::string req = outstr.str();

					//std::cout << "requset: " << req << std::endl;

					//this->handlerProvider.GetHandler()

					LTRACE("Json") << "request " << req << LE;

					if (auto sHandlerProvider = handlerProvider.lock()) {
						sHandlerProvider->getHandler(serviceName)->HandleRequest(req, rsp);
					} else {
						throw Poco::Exception("Request refused");
					}

					LTRACE("Json") << "response " << rsp << LE;
					//std::cout << "response: " << rsp << std::endl;

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
			ServiceHandlers::WeakPtr handlerProvider;
			TBS::Services::JsonServerParams p;
	};



	HttpInterfaceServer::HttpInterfaceServer(const TBS::Services::JsonServerParams & p) :
			p(p)/*, tp(1)*/ {


		Poco::Net::HTTPServerParams* pParams = new Poco::Net::HTTPServerParams;

		if (p.MaxParallelThreads.isSet()){
			pParams->setMaxThreads(p.MaxParallelThreads.val());
		} else {
			pParams->setMaxThreads(2);
		}

		if (p.MaxQueuedThreads.isSet()){
			pParams->setMaxQueued(p.MaxQueuedThreads.val());
		} else {
			pParams->setMaxQueued(100);
		}

		if (p.KeepAliveTimeout.isSet()){
			pParams->setKeepAliveTimeout(Poco::Timespan(p.KeepAliveTimeout.val(),0));
		} else {
			pParams->setKeepAliveTimeout(Poco::Timespan(5,0));
		}

		if (p.isHttps()) {
			std::cout << "service via https: PK: " << p.getHttpsPrivateKey() << " CT:" << p.getHttpsCertificate() << std::endl;
			Poco::Net::Context::Ptr context = new Poco::Net::Context(Poco::Net::Context::SERVER_USE, p.getHttpsPrivateKey(), p.getHttpsCertificate(), "");
			// disable session cache because of Firefox (less memory consuming than session cache enabling)
#ifndef OLDPOCO13
			context->disableStatelessSessionResumption();
#endif
			Poco::Net::SecureServerSocket svs(p.port(), 64, context);
			srv = std::unique_ptr<Poco::Net::HTTPServer>(new Poco::Net::HTTPServer(new RootHandlerFactory<MultiRootHandler>(handlers(), p), /*tp,*/ svs, pParams));
			std::cout << "multi https server listens on " << srv->port() << std::endl;
		} else {
			Poco::Net::ServerSocket svs(p.port()); // set-up a server socket
			srv = std::unique_ptr<Poco::Net::HTTPServer>(new Poco::Net::HTTPServer(new RootHandlerFactory<MultiRootHandler>(handlers(), p), /*tp,*/ svs, pParams));
			std::cout << "multi server listens on " << srv->port() << std::endl;
		}
		//LERROR("Service.Jsonp") << "construct with: " << (int)&handlers_ << " this: " << this << LE;
	}

	HttpInterfaceServer::~HttpInterfaceServer() {
		std::cout << "destruct1" << std::endl;
		srv->stop();

		std::cout << "destruct1a" << std::endl;
		//tp.stopAll();

		std::cout << "destruct2" << std::endl;
		//tp.joinAll();
		std::cout << "destruct3" << std::endl;
		//LERROR("Service.Jsonp") << "destruct with: " << (int)&handlers_ << " this: " << this << LE;
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



} /* namespace jsonrpc */
