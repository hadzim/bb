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

	Json::Value FullNodeDataRW::write(const FullNodeData & sensorData){
		Json::Value v = Json::objectValue;
		v["d"] = NodeDataRW::write(sensorData.data);
		v["n"] = sensorData.node;
		v["s"] = sensorData.sensor;
		return v;
	}

	FullNodeData FullNodeDataRW::read(const Json::Value & value){
		Node::Data data = NodeDataRW::read(value["d"]);
		return FullNodeData(value["n"].asString(), value["s"].asString(), data);
	}


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
			sensorCache("/tmp/cache.sensor"), statusCache("/tmp/cache.status"), notificationCache("/tmp/cache.notification"), nodeDataCache("/tmp/nodedata.notification"), nodeInfoCache("/tmp/nodeinfo.notification"), settings(s) {

	}

	void RemoteClient::setEndpoint(const RemoteClientSettings & s){
		Poco::Mutex::ScopedLock l(settingsMutex);
		this->settings = s;
	}

	void RemoteClient::sendImage(std::string path, std::string info) {
		TBS::Nullable<RemoteClientSettings> s;
		{
			Poco::Mutex::ScopedLock l(settingsMutex);
			s.set(settings);
		}
		Poco::Net::HTTPClientSession session(s.ref().url, s.ref().port);

		Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, s.ref().query + "?presenter=Data%3AInsert&action=binary", Poco::Net::HTTPMessage::HTTP_1_1);

		request.set("origin", "null");
		request.add("accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
		request.setChunkedTransferEncoding(false);

		Poco::Net::HTMLForm form(Poco::Net::HTMLForm::ENCODING_MULTIPART);
		form.set("project", s.ref().projectID);
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

	std::string RemoteClient::remoteQuery(std::string action, std::string data, std::string requestFile) {
		LNOTICE("RemoteClient")<< "Sending: " << data << LE;

		TBS::Nullable<RemoteClientSettings> s;
		{
			Poco::Mutex::ScopedLock l(settingsMutex);
			s.set(settings);
		}

		LNOTICE("RemoteClient")<< "Sending to: " << s.ref().url << " port " << s.ref().port << LE;


		Poco::Net::HTTPClientSession session(s.ref().url, s.ref().port);
		Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, settings.query, "HTTP/1.1");
		Poco::Net::HTMLForm form;
		form.add("presenter", "Data:Insert");
		form.add("action", action);
		//std::string encodedData = "";
		//Poco::URI::encode(data, "#", encodedData);

		form.add("data", data);
		form.add("project", s.ref().projectID);
		form.prepareSubmit(request);

		if (!requestFile.empty()){
			std::ofstream f(requestFile);
			f << s.ref().url << ":" << s.ref().port << std::endl;
			f << settings.query << std::endl;
			f << "project: " <<  s.ref().projectID << std::endl;
			f << "action: " <<  action << std::endl;
			f << "presenter: " <<  "Data:Insert" << std::endl;
			f << "data: " << data << std::endl;
			f << std::endl;
			f << std::endl;
			f << "http://" << s.ref().url << ":" << s.ref().port << "/" << settings.query
					 << "?project=" <<  s.ref().projectID
					 << "&action=" <<  action
					 << "&presenter=" <<  "Data:Insert"
					 << "&data=" << data << std::endl;
			f << std::endl;
			f << std::endl;
			request.write(f);
		}

		std::cout << "request before: " << settings.query << std::endl;

		session.sendRequest(request);

		Poco::Net::HTTPResponse response;
		std::istream& rs = session.receiveResponse(response);

		std::stringstream of;//("errStatus.txt");

		Poco::StreamCopier::copyStream(rs, of);

		LINFO("RemoteClient") << "Received: " << of.str() << LE;

		return of.str();
	}

	void RemoteClient::sendToRemoteServer(const RuntimeStatus & st) {
		std::string res = this->remoteQuery("status", RW::json2String(StatusDataRW::write(st)), "/tmp/lastStatusRequest.txt");
		std::ofstream f("/tmp/lastStatusResponse.txt");
		f << res;
	}

	void RemoteClient::sendToRemoteServer(const SensorData & m) {
		std::string res = this->remoteQuery("sensor", RW::json2String(SensorDataRW::write(m)), "/tmp/lastSensorRequest.txt");
		std::ofstream f("/tmp/lastSensorResponse.txt");
		f << res;
	}
	void RemoteClient::sendToRemoteServer(const Notification & s){
		std::string res = this->remoteQuery("notification", RW::json2String(NotificationDataRW::write(s)), "/tmp/lastNotificationRequest.txt");
		std::ofstream f("/tmp/lastNotificationResponse.txt");
		f << res;
	}
	void RemoteClient::sendToRemoteServer(const FullNodeData & s){
		LNOTICE("RC") << "going to send full data: " << s.data << LE;
		std::string res = this->remoteQuery("nodeSensor", RW::json2String(FullNodeDataRW::write(s)), "/tmp/lastNodeSensorRequest.txt");
		std::ofstream f("/tmp/lastNodeSensorResponse.txt");
		f << res;
	}
	void RemoteClient::sendToRemoteServer(const Node::Info & s){
		LNOTICE("RC") << "going to send: " << s << LE;
		std::string res = this->remoteQuery("nodeInfo", RW::json2String(NodeInfoRW::write(s)), "/tmp/lastNodeInfoRequest.txt");
		std::ofstream f("/tmp/lastNodeInfoResponse.txt");
		f << res;
	}

	void RemoteClient::sendCache() {
		sendCacheImpl<NotificationCache>(notificationCache);
		sendCacheImpl<SensorCache>(sensorCache);
		sendCacheImpl<StatusCache>(statusCache);


		sendCacheImpl<NodeInfoCache>(nodeInfoCache);
		sendCacheImpl<NodeDataCache>(nodeDataCache);

		int size = nodeDataCache.size();
		this->CacheChanged.notify(this, size);
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

	void RemoteClient::forward(const Node::Info & info, const Node::Sensor & sensor, const Node::Data & data){
		LINFO("RC")<<"forward node data" << LE;
		FullNodeData fd(info.getUID(), sensor.name, data);
		forwardImpl<FullNodeData, NodeDataCache>(fd, nodeDataCache);
		int size = nodeDataCache.size();
		this->CacheChanged.notify(this, size);
	}
	void RemoteClient::forward(const Node::Info & data){
		LINFO("RC")<<"forward node info" << LE;
		forwardImpl<Node::Info, NodeInfoCache>(data, nodeInfoCache);
	}

}
/* namespace BB */
