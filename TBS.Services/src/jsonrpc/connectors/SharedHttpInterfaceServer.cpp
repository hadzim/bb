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


	SharedHttpInterfaceServer::SharedHttpInterfaceServer(const TBS::Services::JsonServerParams & p,  FactoryFnc factoryCreator) :
				p(p), tp("jsonpserver", 2, 4, 5) {

			LDEBUG("Service") << "shared http server construct " << this << LE;

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
				srv = std::unique_ptr<Poco::Net::HTTPServer>(new Poco::Net::HTTPServer(factoryCreator(handlers(), p), tp, svs, pParams));
				LDEBUG("Service") << "multi https server listens on " << srv->port() << this << LE;
				std::cout << "multi https server listens on " << srv->port() << std::endl;
			} else {
				Poco::Net::ServerSocket svs(p.port()); // set-up a server socket
				srv = std::unique_ptr<Poco::Net::HTTPServer>(new Poco::Net::HTTPServer(factoryCreator(handlers(), p), tp, svs, pParams));
				LDEBUG("Service") << "multi http server listens on " << srv->port() << this << LE;
			}
		}

	SharedHttpInterfaceServer::~SharedHttpInterfaceServer() {
			try {
				LDEBUG("Service") << "http server destruct " << this << LE;
				//LERROR("Service.Jsonp") << "destruct with: " << (int)&handlers_ << " this: " << this << LE;
				std::cout << "destruct1" << std::endl;
				srv->stop();

				LTRACE("Service") << "http server destruct 1 " << this << LE;


				std::cout << "destruct1a" << std::endl;
				tp.stopAll();

				LTRACE("Service") << "http server destruct 2 " << this << LE;


				std::cout << "destruct2" << std::endl;

				LTRACE("Service") << "http server destruct 3 " << this << LE;


				tp.joinAll();
				std::cout << "destruct3" << std::endl;

				LTRACE("Service") << "http server destruct 4 " << this << LE;


				srv.reset();

				LDEBUG("Service") << "http server destructed " << this << LE;
			} catch (Poco::Exception & e){
				LWARNING("Service") << "http server destructed exc " << e.displayText() << LE;
			} catch (...){
				LWARNING("Service") << "http server destructed exc " << this << LE;
			}
		}

		bool SharedHttpInterfaceServer::StartListening() {
			LTRACE("Service") << "start listening: " << LE;
			std::cout << "multi server start listening " << std::endl;
			srv->start();
			return true;
		}

		bool SharedHttpInterfaceServer::StopListening() {
			LTRACE("Service") << "stop listening: " << LE;
			std::cout << "multi server stop listening " << std::endl;
			srv->stop();
			return true;

		}


} /* namespace jsonrpc */
