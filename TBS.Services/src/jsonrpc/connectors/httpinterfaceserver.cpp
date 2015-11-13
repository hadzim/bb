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

				jsonrpc::handleRequest(response, [&](){

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
						throw Poco::Exception("Request refused - server destroyed");
					}

					LTRACE("Json") << "response " << rsp << LE;
					//std::cout << "response: " << rsp << std::endl;

					response.setContentType("application/json");
					response.sendBuffer(rsp.data(), rsp.length());
				});
			}
		private:
			ServiceHandlers::WeakPtr handlerProvider;
			TBS::Services::JsonServerParams p;
	};


	static Poco::Net::HTTPRequestHandlerFactory::Ptr createHttpJsonRPC(ServiceHandlers::Ptr h, const TBS::Services::JsonServerParams & p){
		return new RootHandlerFactory<MultiRootHandler>(h, p);
	}

	HttpInterfaceServer::HttpInterfaceServer(const TBS::Services::JsonServerParams & p) :
		SharedHttpInterfaceServer(p, createHttpJsonRPC){

	}




} /* namespace jsonrpc */
