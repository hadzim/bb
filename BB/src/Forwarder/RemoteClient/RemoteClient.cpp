/*
 * RemoteClient.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#include "BB/Forwarder/RemoteClient/RemoteClient.h"
#include <iostream>
#include <Poco/Delegate.h>
#include "Poco/Net/HTMLForm.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/FilePartSource.h"
#include "BB/DataRW.h"
#include "TBS/Log.h"
#include <fstream>

#include "Poco/URI.h"

#include "json/value.h"

#include "json/reader.h"

#include "BB/Services/DataSvc_DBus.h"

#include "BB/Configuration.h"
#include <Poco/Net/MultipartWriter.h>

namespace BB {

	RemoteClientSettings::RemoteClientSettings() {
		url = Configuration::initCfg("RemoteClient", "url", "mereni.hadzim.net");
		port = Configuration::initCfg("RemoteClient", "port", 80);
		query = Configuration::initCfg("RemoteClient", "query", "/");
		projectID = Configuration::initCfg("RemoteClient", "projectID", "1");

	}
	RemoteClientSettings::RemoteClientSettings(std::string url, int port, std::string query, std::string project) :
			url(url), port(port), query(query), projectID(project) {

	}

	RemoteClient::RemoteClient(RemoteClientSettings s) :
			sensorCache("/tmp/cache.sensor"), statusCache("/tmp/cache.status"), notificationCache("/tmp/cache.notification"), settings(s) {

	}

	void RemoteClient::sendImage(std::string path, std::string info) {

		Poco::Net::HTTPClientSession session(settings.url, settings.port);

		Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, settings.query + "?presenter=Data%3AInsert&action=binary", Poco::Net::HTTPMessage::HTTP_1_1);

		request.set("origin", "null");
		request.add("accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
		request.setChunkedTransferEncoding(false);

		Poco::Net::HTMLForm form(Poco::Net::HTMLForm::ENCODING_MULTIPART);
		form.set("project", settings.projectID);
		form.set("info", info);
		form.addPart("image", new Poco::Net::FilePartSource(path, "image/jpeg"));

		std::string boundary = Poco::Net::MultipartWriter::createBoundary();
		std::string ct("multipart/form-data");
		ct.append("; boundary=");
		ct.append(boundary);
		ct.append("");
		request.setContentType(ct);

		std::stringstream sstr;

		form.write(sstr, boundary);
		int length = sstr.str().length();

		request.setContentLength(length);
		request.set("User-Agent", "BB.Client");
		request.set("host", settings.url);

		session.sendRequest(request) << sstr.str();

		Poco::Net::HTTPResponse response;
		std::istream& rs = session.receiveResponse(response);

		std::stringstream of;						//("errStatus.txt");

		Poco::StreamCopier::copyStream(rs, of);

		std::ofstream ofw("/tmp/cam.err");
		ofw << of.str();

		std::cout << "sent and received: " << of.str() << std::endl;
		LDEBUG("BIN") << "sent and received: " << of.str() << LE;
	}

	std::string RemoteClient::remoteQuery(std::string action, std::string data) {
		LINFO("RemoteClient")<< "Sending: " << data << LE;

			Poco::Net::HTTPClientSession s(settings.url, settings.port);
			Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, settings.query);
			Poco::Net::HTMLForm form;
			form.add("presenter", "Data:Insert");
			form.add("action", action);
			//std::string encodedData = "";
			//Poco::URI::encode(data, "#", encodedData);

			form.add("data", data);
			form.add("project", settings.projectID);
			form.prepareSubmit(request);

			std::cout << "request before: " << settings.query << std::endl;

			s.sendRequest(request);

			Poco::Net::HTTPResponse response;
			std::istream& rs = s.receiveResponse(response);

			std::stringstream of;//("errStatus.txt");

			Poco::StreamCopier::copyStream(rs, of);

			LINFO("RemoteClient") << "Received: " << of.str() << LE;

			return of.str();
		}

	void RemoteClient::sendToRemoteServer(const RuntimeStatus & st) {
		std::string res = this->remoteQuery("status", RW::json2String(StatusDataRW::write(st)));
		std::ofstream f("/tmp/lastStatus.txt");
		f << res;
	}

	void RemoteClient::sendToRemoteServer(const SensorData & m) {
		std::string res = this->remoteQuery("sensor", RW::json2String(SensorDataRW::write(m)));
		std::ofstream f("/tmp/lastSensor.txt");
		f << res;
	}
	void RemoteClient::sendToRemoteServer(const Notification & s){
		std::string res = this->remoteQuery("notification", RW::json2String(NotificationDataRW::write(s)));
		std::ofstream f("/tmp/lastNotification.txt");
		f << res;
	}


	void RemoteClient::sendCache() {
		sendCacheImpl<NotificationCache>(notificationCache);
		sendCacheImpl<SensorCache>(sensorCache);
		sendCacheImpl<StatusCache>(statusCache);
	}

	void RemoteClient::forward(const SensorData & m) {
		forwardImpl<SensorData, SensorCache>(m, sensorCache);

	}
	void RemoteClient::forward(const RuntimeStatus & data) {
		LINFO("RC")<<"forward status" << LE;
		forwardImpl<RuntimeStatus, StatusCache>(data, statusCache);

	}
	void RemoteClient::forward(const Notification & data){
		LINFO("RC")<<"forward ntf" << LE;
		forwardImpl<Notification, NotificationCache>(data, notificationCache);
	}

}
/* namespace BB */
