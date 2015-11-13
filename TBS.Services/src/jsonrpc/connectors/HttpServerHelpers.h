/*
 * HttpServerHelpers.h
 *
 *  Created on: May 29, 2014
 *      Author: dron
 */

#ifndef HTTPSERVERHELPERS_H_
#define HTTPSERVERHELPERS_H_
#include "Poco/MD5Engine.h"

#include "Poco/DigestStream.h"

#include "jsonrpc/connectors/httpinterfaceserver.h"

#include "Poco/Net/HTTPServerRequest.h"

#include "Poco/Net/HTTPServerResponse.h"

#include "Poco/Net/HTTPBasicCredentials.h"
#include "TBS/Services/Json/JsonServices.h"

#include "TBS/Log.h"
#include <functional>
namespace jsonrpc {

	class HttpServerHelpers {
		public:
			HttpServerHelpers();
			virtual ~HttpServerHelpers();

			enum ReturnType {
				RequestOk, RequestFinished
			};



			static ReturnType preprocess(TBS::Services::JsonServerParams params, Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
	};

	template <class RootHandler>
	class RootHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
		public:
			RootHandlerFactory(ServiceHandlers::WeakPtr handlerProvider, const TBS::Services::JsonServerParams & p) :
					handlerProvider(handlerProvider), p(p) {
				LDEBUG("Json") << "Root handler factory constructed " << this << LE;
			}
			virtual ~RootHandlerFactory() {

				LDEBUG("Json") << "Root handler factory deleted" << this << LE;

				//TBS::dumpBacktrace("Json", "Json", 6);
			}
			Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) {
				return new RootHandler(handlerProvider, p);
			}
		private:
			ServiceHandlers::WeakPtr handlerProvider;
			TBS::Services::JsonServerParams p;
	};



	void handleRequest(Poco::Net::HTTPServerResponse& response, std::function<void()> fnc);



} /* namespace jsonrpc */

#endif /* HTTPSERVERHELPERS_H_ */
