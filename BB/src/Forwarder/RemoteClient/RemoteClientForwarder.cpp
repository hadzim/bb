/*
 * RemoteClientForwarder.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#include "BB/Forwarder/RemoteClient/RemoteClientForwarder.h"
#include <iostream>
#include <Poco/Delegate.h>
#include "Poco/Net/HTMLForm.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "BB/Sensor/SensorDataRW.h"
#include "TBS/Log.h"

namespace BB {

	RemoteClientForwarder::RemoteClientForwarder(std::string host, std::string requestUrl, std::string projectID)
			: cache("cache.RemoteClientForwarder"), host(host), requestUrl(requestUrl), projectID(projectID) {
		//std::cout << "fwd created" << std::endl;
	}

	RemoteClientForwarder::~RemoteClientForwarder() {

	}

	void RemoteClientForwarder::sendToRemoteServer(const SensorData & m){
		Poco::Net::HTTPClientSession s(host);
		Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, requestUrl);
		Poco::Net::HTMLForm form;
		form.add("presenter", "Insert");
		form.add("data", SensorDataRW::write2String(m));
		form.add("project", projectID);
		form.prepareSubmit(request);
		s.sendRequest(request);

		Poco::Net::HTTPResponse response;
		std::istream& rs = s.receiveResponse(response);
		Poco::StreamCopier::copyStream(rs, std::cout);

	}

	void RemoteClientForwarder::sendCache(){
		Cache::Data data = cache.fetch();

		int cnt = 0;
		for (Cache::Data::iterator i = data.begin(); i != data.end(); i++){
			try {
				sendToRemoteServer(*i);
				cnt++;
			} catch (Poco::Exception & e){
				LWARNING("RemoteClient") << "RemoteClientForwarder: send cached data failed: " << e.displayText() << LE;
				std::cerr << "send cache data: " <<  e.displayText() << std::endl;
				break;
			}
		}
		if (cnt){
			cache.removeFirst(cnt);
		}

	}

	void RemoteClientForwarder::forward(const SensorData & m) {
		std::cout << "filter data RC" << std::endl;
		bool ok = false;
		try {
			this->sendToRemoteServer(m);
			ok = true;
		} catch (Poco::Exception & e){
			LWARNING("RemoteClient") << "RemoteClientForwarder: send data failed: " << e.displayText() << LE;
			std::cerr << "send data: " <<  e.displayText() << std::endl;
			cache.store(m);
		}

		if (ok){
			sendCache();
		}
	}

} /* namespace BB */
