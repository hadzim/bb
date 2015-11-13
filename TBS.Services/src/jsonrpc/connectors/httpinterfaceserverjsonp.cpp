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

#include "TBS/Log.h"

#include "HttpServerHelpers.h"

namespace jsonrpc {



	class JsonpMultiRootHandler: public Poco::Net::HTTPRequestHandler {
		public:

			JsonpMultiRootHandler(ServiceHandlers::WeakPtr serviceHandlers, TBS::Services::JsonServerParams p) :
					handlerProvider(serviceHandlers), p(p) {
				//LERROR("Service.Jsonp") << "reference holder construct: obj" << (int)this << " ref: " << (int)&this->handlerProvider << LE;
			}
			~JsonpMultiRootHandler(){
				//LERROR("Service.Jsonp") << "reference holder destruct: obj" << (int)this << " ref: " << (int)&this->handlerProvider << LE;
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
						LERROR("Json") << "Cannot parse response: " << respOriginalStr << LE;
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
				jsonrpc::handleRequest(response, [&](){

					LTRACE("Service.Jsonp") << "requset: " << request.getURI() << LE;

					HttpServerHelpers::ReturnType ret = HttpServerHelpers::preprocess(p, request, response);
					if (ret == HttpServerHelpers::RequestFinished){
						return;
					}

					std::string serviceName = request.getURI();

					if (serviceName.find('/') == 0) {
						serviceName = serviceName.substr(1);
					}
					std::string::size_type question = serviceName.find('?');
					if (question != std::string::npos) {
						serviceName = serviceName.substr(0, question);
					}

					for (auto i = p.getRequestHandlers().begin(); i != p.getRequestHandlers().end(); i++){
						if ((*i)->canHandle(serviceName)){
							(*i)->handle(request, response);
							return;
						}
					}

					//std::cout << "service name: " << serviceName << std::endl;

					Poco::Net::HTMLForm form(request, request.stream());

					 Json::Value jsrequest(Json::objectValue);

					 jsrequest["jsonrpc"] = "2.0";
					 jsrequest["params"] = Json::Value(Json::objectValue);
					 jsrequest["id"] = 99;
					 jsrequest["method"] = form["method"];


					 std::string callbackFnc = form["callback"];

					for (Poco::Net::HTMLForm::ConstIterator i = form.begin(); i != form.end(); i++){
						LTRACE("Service.Jsonp") << "param: " << i->first << " value: " << i->second << LE;
						//std::cout << "param: " << i->first << " value: " << i->second << std::endl;
						if (i->first == "callback" || i->first == "_" || i->first == "method"){
							continue;
						}
						jsrequest["params"][i->first] = i->second;
					}

					 Json::FastWriter w;
					 std::string jsonstrrequest = w.write(jsrequest);


					std::cout << "requset: " << jsonstrrequest << std::endl;
					LTRACE("Service.Jsonp") << "requset: " << jsonstrrequest << LE;

					//this->handlerProvider.GetHandler()
					std::string rsp;
					//LERROR("DBG") << (int)&this->handlerProvider << " before get "  << LE;

					if (auto sHandlerProvider = handlerProvider.lock()) {
						sHandlerProvider->getHandler(serviceName)->HandleRequest(jsonstrrequest, rsp, true);
					} else {
						throw Poco::Exception("Request refused");
					}

					//output from Json RPC to Cristis format
					rsp = parseResponse(rsp);


					rsp = callbackFnc + "(" + rsp + ")";

					LTRACE("Service.Jsonp") << "response: " << rsp << LE;

					response.setContentType("application/json");
					response.sendBuffer(rsp.data(), rsp.length());
				});
			}
		private:
			ServiceHandlers::WeakPtr handlerProvider;
			TBS::Services::JsonServerParams p;
	};

	static Poco::Net::HTTPRequestHandlerFactory::Ptr createHttpJsonp(ServiceHandlers::Ptr h, const TBS::Services::JsonServerParams & p){
			return new RootHandlerFactory<JsonpMultiRootHandler>(h, p);
		}


	JsonpHttpInterfaceServer::JsonpHttpInterfaceServer(const TBS::Services::JsonServerParams & p) :
		SharedHttpInterfaceServer(p, createHttpJsonp){

	}



} /* namespace jsonrpc */
