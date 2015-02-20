#include "DotNet/DotNetClient.h"

#include <Poco/SharedPtr.h>
#include <Poco/BasicEvent.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <map>
#include <Poco/StreamCopier.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPClientSession.h>
#include <memory>
#include <json/reader.h>
#include "TBS/Log.h"

namespace DotNet {

	DotNetClient::DotNetClient(const TBS::Services::DotNetClientParams & params) :
			params(params) {

		LTRACE("Camera") << "construct: "  << "create client " << params.host << ":" << params.port << LE;

		std::cout << "create client " << params.host << ":" << params.port << std::endl;
		client = ClientPtr(new Poco::Net::HTTPClientSession(params.host, params.port));
		if (params.timeout.totalMilliseconds() > 0) {
			client->setTimeout(params.timeout);
		}
		std::cout << "create client " << params.host << ":" << params.port << " done" << std::endl;

		LTRACE("Camera") << "construct: "  << "create client " << params.host << ":" << params.port  << " done" << LE;

		LTRACE("Camera") << "client timeout: " << client->getTimeout().totalSeconds() << LE;

	}


	Json::Value DotNetClient::sendRequest(std::string url) {

		//TODO remove log lines
		LDEBUG("Camera") << "sendRequest: " << url << LE;

		std::string sendUrl = params.query + url;

		LDEBUG("Camera") << "sendRequest full: " << sendUrl << LE;

		std::cout << "query " << sendUrl << std::endl;

		Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, sendUrl);
		request.setContentType("application/json");
		request.set("charsets:", "utf-8");
		request.setContentLength(0); //(int) message.length()
		client->sendRequest(request);
		Poco::Net::HTTPResponse response;
		std::istream& rs = client->receiveResponse(response);
		std::stringstream outstr;
		Poco::StreamCopier::copyStream(rs, outstr);
		std::string rawOutput = outstr.str();
		std::cout << "response " << rawOutput << std::endl;
		LDEBUG("Camera") << "resposne raw: " << rawOutput << LE;
		Json::Value ov;
		Json::Reader r;
		r.parse(rawOutput, ov);

		//LDEBUG("Camera") << "parsed: " << ov << LE;

		return ov;
	}

}

