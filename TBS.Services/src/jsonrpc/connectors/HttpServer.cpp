#include "jsonrpc/connectors/HttpServer.h"

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



	class HttpHandler: public Poco::Net::HTTPRequestHandler {
		public:

			HttpHandler(ServiceHandlers::WeakPtr serviceHandlers, TBS::Services::JsonServerParams & p) :
					handlerProvider(serviceHandlers), p(p) {
			}
			~HttpHandler(){
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

					throw Poco::FileNotFoundException("File not found");

				});
			}
		private:
			ServiceHandlers::WeakPtr handlerProvider;
			TBS::Services::JsonServerParams p;
	};


	static Poco::Net::HTTPRequestHandlerFactory::Ptr createHttp(ServiceHandlers::Ptr h, const TBS::Services::JsonServerParams & p){
		LDEBUG("Json") << "create http" << LE;

		return new RootHandlerFactory<HttpHandler>(h, p);
	}

	HttpServer::HttpServer(const TBS::Services::JsonServerParams & p) :
		SharedHttpInterfaceServer(p, createHttp){

	}




} /* namespace jsonrpc */
