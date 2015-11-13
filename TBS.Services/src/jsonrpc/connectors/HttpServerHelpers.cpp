/*
 * HttpServerHelpers.cpp
 *
 *  Created on: May 29, 2014
 *      Author: dron
 */

#include "HttpServerHelpers.h"
#include "jsonrpc/connectors/helpers.h"

#include "TBS/Log.h"
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

	HttpServerHelpers::ReturnType HttpServerHelpers::preprocess(TBS::Services::JsonServerParams params, Poco::Net::HTTPServerRequest& request,
	        Poco::Net::HTTPServerResponse& response) {
		std::string serviceName = request.getURI();

		bool isRoot = serviceName == "/";

		LTRACE("Json") << "method: " << request.getMethod() << " url: " << request.getURI() <<  LE;

		if (params.isCrossDomainAllowed() && !isRoot) {
			try {
				response.set("Access-Control-Allow-Origin", request.get("Origin"));
			} catch (Poco::Exception & e) {
				response.set("Access-Control-Allow-Origin", "*");
			}
			response.set("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
			response.set("Access-Control-Max-Age", "1000");
			response.set("Access-Control-Allow-Headers", "origin, x-csrftoken, content-type, accept, authorization, X-Requested-With");
			response.set("Access-Control-Allow-Credentials", "true");
		}
		if (request.getMethod() == "OPTIONS") {
			response.send();
			return RequestFinished;
		}

		if (params.isProtected(	)) {
			//std::cout << "protected" << std::endl;
			if (!request.hasCredentials()) {
				std::cout << "protected - no credentials" << std::endl;
				response.requireAuthentication("TBS Service Authentication");
				response.setContentLength(0);
				response.send();
				return RequestFinished;
			} else {
				//std::cout << "protected - credentials" << std::endl;
				Poco::Net::HTTPBasicCredentials cred(request);
				std::string user = cred.getUsername();
				std::string pwd = cred.getPassword();

				std::string userTarget = params.getUserName();
				//std::string pwdTarget = params.getPasswordHash();

				//std::cout << "protected - match " << user << ":" << userTarget << " and apss " << md5Hash(pwd) << " vs " << pwdTarget << std::endl;



				if (user != userTarget || !params.isPasswordValid(pwd)) {
					std::cout << "protected - bad credentials" << std::endl;

					LTRACE("Json") << "protected - bad credentials" << LE;

					response.requireAuthentication("TBS Service Authentication");
					response.setContentLength(0);
					response.send();
					return RequestFinished;
				} else {
					LTRACE("Json") << "protected - GOOD credentials" << LE;
				}
			}

		}
		return RequestOk;
	}

	void handleRequest(Poco::Net::HTTPServerResponse& response, std::function<void()> fnc){
			try {
				if (fnc){
					fnc();
				}
			} catch (Poco::FileNotFoundException & e){
				LWARNING("Json") << "Server exception: " << e.displayText() << LE;
				response.setStatusAndReason(Poco::Net::HTTPServerResponse::HTTP_BAD_REQUEST, e.message());
			} catch (Poco::Exception & e){
				LWARNING("Json") << "Server exception: " << e.displayText() << LE;
				response.setStatusAndReason(Poco::Net::HTTPServerResponse::HTTP_INTERNAL_SERVER_ERROR, e.message());
			} catch (std::exception & e){
				LWARNING("Json") << "Server exception: " << e.what() << LE;
				response.setStatusAndReason(Poco::Net::HTTPServerResponse::HTTP_INTERNAL_SERVER_ERROR, e.what());
			} catch (...){
				LWARNING("Json") << "Server exception: " << "???" << LE;
				response.setStatusAndReason(Poco::Net::HTTPServerResponse::HTTP_INTERNAL_SERVER_ERROR, "unknown server exception");
			}
		}


} /* namespace jsonrpc */
