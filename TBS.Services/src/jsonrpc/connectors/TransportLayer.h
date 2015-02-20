/*
 * TransportLayer.h
 *
 *  Created on: Nov 6, 2014
 *      Author: dron
 */

#ifndef TRANSPORTLAYER_H_
#define TRANSPORTLAYER_H_

#include "TransportLayerHelper.h"
#include "TBS/Nullable.h"

#include <Poco/UUIDGenerator.h>
#include <TBS/Services/ITransportLayer.h>
#include <TBS/Services/Serial/SerialServices.h>

namespace TBS {
	namespace Services {



		class PlainClient: public ITransportClient {
			public:
				PlainClient(ITransportLayer::Ptr transport) :
						transport(transport) {

				}
				virtual ~PlainClient() {

				}

				virtual std::string sendRequest(const std::string & request, const std::string interface) {
					LTRACE("rpc") << "plain client send " << request << " inteface " << interface << LE;

					TBS::Services::TransportLayerHelper::DataMessage m;
					m.message = request;
					m.interface = interface;

					std::string reqRaw = helper.write(helper.createDataMessage(m.interface, m.message));

					LTRACE("rpc") << "plain client RAW send " << reqRaw << LE;

					transport->write(reqRaw);
					std::string respRaw = transport->read();

					LTRACE("rpc") << "plain client read RAW " << respRaw << LE;


					TBS::Services::TransportLayerHelper::Message resp = helper.read(respRaw);
					TBS::Services::TransportLayerHelper::DataMessage dm = helper.parseDataMessage(resp);

					LTRACE("rpc") << "plain client received: " << dm.message << LE;

					return dm.message;
				}
			private:
				TBS::Services::TransportLayerHelper helper;
				ITransportLayer::Ptr transport;
		};

		class PlainServer: public ITransportServer {
			public:
				PlainServer(ITransportLayer::Ptr transport) :
						transport(transport) {

				}
				virtual ~PlainServer() {
				}

				virtual void loop() {

					std::string rawRequest = transport->read();

					if (!rawRequest.empty()) {

						LTRACE("rpc") << "plain server RAW read" << rawRequest << LE;


						TBS::Services::TransportLayerHelper::Message req = helper.read(rawRequest);

						TBS::Services::TransportLayerHelper::DataMessage dataMessage = helper.parseDataMessage(req);

						Message msg;
						msg.interface = dataMessage.interface;
						msg.request = dataMessage.message;

						LTRACE("rpc") << "plain server request: " << msg.request << " interface " << msg.interface << LE;

						this->OnMessage.notify(this, msg);

						LTRACE("rpc") << "plain server response: " << msg.response << LE;

						std::string respRaw = helper.write(helper.createDataMessage(msg.interface, msg.response));

						LTRACE("rpc") << "plain server RAW response: " << respRaw << LE;
						transport->write(respRaw);
					}
				}

			private:
				TBS::Services::TransportLayerHelper helper;
				ITransportLayer::Ptr transport;
		};

		class SecureClient: public ITransportClient {
			public:

				struct SecureParams {
						std::string id;
						std::string masterKey;
				};

				SecureClient(ITransportLayer::Ptr transport, SecureParams params) :
						transport(transport), params(params), counter(1) {
					//do handshake

					LTRACE("rpc") << "secure client handshake" << LE;

					//write handlshake message
					std::string reqRaw = helper.write(helper.createHandshakeMessage(params.id));
					transport->write(reqRaw);

					//read session key
					std::string respRaw = transport->read();
					TBS::Services::TransportLayerHelper::Message resp = helper.read(respRaw);

					if (!helper.isSession(resp)) {
						throw Poco::Exception("Wrong message type retrieved");
					}

					TBS::Services::TransportLayerHelper::SessionMessage sm = helper.parseSessionMessage(resp, params.masterKey);

					if (sm.id != params.id) {
						throw Poco::Exception("Message id mismatch");
					}

					this->sessionKey.set(sm.sessionKey);

					LTRACE("rpc") << "secure client handshake done: ses key: " << sm.sessionKey << LE;
				}
				virtual ~SecureClient() {

				}

				virtual std::string sendRequest(const std::string & request, const std::string interface) {
					LTRACE("rpc") << "secure client send " << request << " inteface " << interface << LE;

					TBS::Services::TransportLayerHelper::DataMessage m;
					m.message = request;
					m.interface = interface;

					int writtenWith = counter++;

					std::string reqRaw = helper.write(helper.createEncryptedDataMessage(params.id, interface, request, this->sessionKey.val(), writtenWith));

					LTRACE("rpc") << "secure client RAW send " << reqRaw << LE;


					transport->write(reqRaw);
					std::string respRaw = transport->read();

					LTRACE("rpc") << "secure client read RAW " << respRaw << LE;

					TBS::Services::TransportLayerHelper::Message resp = helper.read(respRaw);

					if (!helper.isEncryptedData(resp)) {
						throw Poco::Exception("Wrong message type retrieved");
					}

					TBS::Services::TransportLayerHelper::EncryptedDataMessage dm = helper.parseEncryptedDataMessage(resp, this->sessionKey.val());

					if (dm.counter != writtenWith) {
						throw Poco::Exception("Message counter mismatch");
					}

					LTRACE("rpc") << "secure client received: " << dm.data.message << LE;

					return dm.data.message;
				}

			private:
				TBS::Services::TransportLayerHelper helper;
				ITransportLayer::Ptr transport;
				SecureParams params;
				TBS::Nullable<std::string> sessionKey;
				int counter;
		};

		class SecureServer: public ITransportServer {
			public:

				struct SecureParams {
						std::string masterKey;
				};

				SecureServer(ITransportLayer::Ptr transport, SecureParams params) :
						transport(transport), params(params) {

				}
				virtual ~SecureServer() {

				}

				virtual void loop() {

					std::string rawRequest = transport->read();

					if (!rawRequest.empty()) {
						LTRACE("rpc") << "secure server RAW read" << rawRequest << LE;

						TBS::Services::TransportLayerHelper::Message req = helper.read(rawRequest);

						if (helper.isData(req)) {
							//do nothing
						}

						if (helper.isHandshake(req)) {
							this->processHandshake(req);
						}

						if (helper.isEncryptedData(req)) {
							this->processData(req);
						}
					}

				}
			private:

				struct ClientInfo {
						std::string sessionKey;
						int counter;
				};

				ClientInfo registerClient(std::string id) {
					ClientInfo info;
					info.counter = 0;
					info.sessionKey = generator.createRandom().toString();
					sessions[id] = info;
					return info;
				}

				void checkSession(std::string id) {
					if (sessions.find(id) == sessions.end()) {
						throw Poco::Exception("Session not established");
					}
				}

				ClientInfo & getSessionInfo(std::string id) {
					checkSession(id);
					return this->sessions[id];
				}

				void processHandshake(TBS::Services::TransportLayerHelper::Message & r) {
					TBS::Services::TransportLayerHelper::HandshakeMessage handshakeMessage = helper.parseHandshakeMessage(r);

					ClientInfo info = this->registerClient(handshakeMessage.id);

					std::string respRaw = helper.write(helper.createSessionMessage(handshakeMessage.id, info.sessionKey, params.masterKey));
					transport->write(respRaw);
				}

				void processData(TBS::Services::TransportLayerHelper::Message & req) {

					std::string id = helper.idFromMessage(req);
					ClientInfo & info = this->getSessionInfo(id);

					TBS::Services::TransportLayerHelper::EncryptedDataMessage dataMessage = helper.parseEncryptedDataMessage(req, info.sessionKey);

					if (info.counter >= dataMessage.counter) {
						throw Poco::Exception("Wrong counter sequence");
					}
					info.counter = dataMessage.counter;

					Message msg;
					msg.interface = dataMessage.data.interface;
					msg.request = dataMessage.data.message;

					LTRACE("rpc") << "plain server request: " << msg.request << " interface " << msg.interface << LE;

					this->OnMessage.notify(this, msg);

					LTRACE("rpc") << "plain server response: " << msg.response << LE;

					std::string respRaw = helper.write(
					        helper.createEncryptedDataMessage(id, msg.interface, msg.response, info.sessionKey, dataMessage.counter));
					transport->write(respRaw);

					LTRACE("rpc") << "plain server RAW response: " << respRaw << LE;
				}

			private:

				typedef std::map<std::string, ClientInfo> Sessions;

				Sessions sessions;

				TBS::Services::TransportLayerHelper helper;
				ITransportLayer::Ptr transport;
				SecureParams params;

				Poco::UUIDGenerator generator;

		};

		ITransportClient::Ptr createClient(ITransportLayer::Ptr t, const TransportLayerParams & params);
		ITransportServer::Ptr createServer(ITransportLayer::Ptr t, const TransportLayerParams & params);

	} /* namespace Services */
} /* namespace TBS */

#endif /* TRANSPORTLAYER_H_ */
