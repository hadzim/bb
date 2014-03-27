/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    httpserver.cpp
 * @date    31.12.2012
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "jsonrpc/connectors/httpinterfaceserverjsonp.h"
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

#include "Poco/Net/HTMLForm.h"

#include "Poco/Net/HTTPServer.h"
#include "Poco/URI.h"
#include <Poco/StreamCopier.h>
#include "jsonrpc/ServiceHandlers.h"

#include "Poco/MD5Engine.h"
#include "Poco/DigestStream.h"
#include "Poco/DigestEngine.h"

#include "Poco/File.h"
#include <fstream>

namespace jsonrpc {



	class JsonpMultiRootHandler: public Poco::Net::HTTPRequestHandler {
		public:

			JsonpMultiRootHandler(ServiceHandlers & serviceHandlers, HttpServerParams p) :
					handlerProvider(serviceHandlers), p(p) {
			}

			static std::string parseResponse(std::string respOriginalStr){

				Json::Value respFinalOk;
				std::string errorMessage;

		        try {
			        Json::Value respOriginalJson;
		        	Json::Reader reader;
					if (reader.parse(respOriginalStr, respOriginalJson, false))
					{
						if (respOriginalJson["error"].isNull()){
							//valid data
							respFinalOk = respOriginalJson["result"];
						} else {
							errorMessage = respOriginalJson["error"]["message"].asString();
						}
					} else {
						errorMessage = "Output not in json format";
					}
		    	} catch (Poco::Exception & e){
					errorMessage = e.message();
					std::cout << "Exception: " << e.displayText() << std::endl;
				} catch (std::exception & e){
					std::cout << "Exception: " << e.what() << std::endl;
					errorMessage = e.what();
				} catch (...){
					std::cout << "Exception: ??" << std::endl;
					errorMessage = "Unknown error";
				}

				Json::Value val;
				val["ReturnCode"] = errorMessage.empty() ? 200 : 500;
				val["ReturnMessage"] = errorMessage.empty() ? "OK" : errorMessage;
				val["ReturnObject"] = respFinalOk;

				 Json::FastWriter writer;
				return writer.write(val);
			}




			void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) {
				std::string errorMessage = "";
				try {

					std::string serviceName = request.getURI();

					bool isRoot = serviceName == "/";

					std::cout << "SERVER service name: " << serviceName << std::endl;
					std::cout << "SERVER request length: " << request.getContentLength() << std::endl;
					std::cout << "SERVER request method: " << request.getMethod() << std::endl;
					std::cout << "SERVER request chunked: " << request.getChunkedTransferEncoding() << std::endl;
					std::cout << "SERVER request type: " << request.getContentType() << std::endl;
					std::cout << "SERVER request host: " << request.getHost() << std::endl;

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
						serviceName = serviceName.substr(0, question);
					}


					std::cout << "service name: " << serviceName << std::endl;

					Poco::Net::HTMLForm form(request, request.stream());

					 Json::Value jsrequest(Json::objectValue);

					 jsrequest["jsonrpc"] = "2.0";
					 jsrequest["params"] = Json::Value(Json::objectValue);
					 jsrequest["id"] = 99;
					 jsrequest["method"] = form["method"];


					 std::string callbackFnc = form["callback"];

					for (Poco::Net::HTMLForm::ConstIterator i = form.begin(); i != form.end(); i++){
						std::cout << "param: " << i->first << " value: " << i->second << std::endl;
						if (i->first == "callback" || i->first == "_" || i->first == "method"){
							continue;
						}
						jsrequest["params"][i->first] = i->second;
					}

					 Json::FastWriter w;
					 std::string jsonstrrequest = w.write(jsrequest);


					std::cout << "requset: " << jsonstrrequest << std::endl;

					//this->handlerProvider.GetHandler()
					std::string rsp;
					this->handlerProvider.getHandler(serviceName)->HandleRequest(jsonstrrequest, rsp, true);

					//output from Json RPC to Cristis format
					rsp = parseResponse(rsp);


					rsp = callbackFnc + "(" + rsp + ")";

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

	class JsonpMultiRequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory {
		public:
			JsonpMultiRequestHandlerFactory(ServiceHandlers & handlerProvider, const HttpServerParams & p) :
					handlerProvider(handlerProvider), p(p) {
				//std::cout < "hfactory " << &handlerProvider << std::endl;
			}
			virtual ~JsonpMultiRequestHandlerFactory() {
			}
			Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) {
				return new JsonpMultiRootHandler(handlerProvider, p);
			}
		private:
			ServiceHandlers & handlerProvider;
			HttpServerParams p;
	};

	JsonpHttpInterfaceServer::JsonpHttpInterfaceServer(const HttpServerParams & p) :
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
			srv = std::auto_ptr<Poco::Net::HTTPServer>(new Poco::Net::HTTPServer(new JsonpMultiRequestHandlerFactory(handlers_, p), svs, pParams));
			std::cout << "multi https server listens on " << srv->port() << std::endl;
		} else {
			Poco::Net::ServerSocket svs(p.port); // set-up a server socket
			srv = std::auto_ptr<Poco::Net::HTTPServer>(new Poco::Net::HTTPServer(new JsonpMultiRequestHandlerFactory(handlers_, p), svs, pParams));
			std::cout << "multi server listens on " << srv->port() << std::endl;
		}
	}

	JsonpHttpInterfaceServer::~JsonpHttpInterfaceServer() {
		srv->stop();
	}

	bool JsonpHttpInterfaceServer::StartListening() {
		std::cout << "multi server start listening " << std::endl;
		srv->start();
		return true;
	}

	bool JsonpHttpInterfaceServer::StopListening() {
		std::cout << "multi server stop listening " << std::endl;
		srv->stop();
		return true;

	}
	ServiceHandlers & JsonpHttpInterfaceServer::handlers() {
		return this->handlers_;
	}

} /* namespace jsonrpc */
