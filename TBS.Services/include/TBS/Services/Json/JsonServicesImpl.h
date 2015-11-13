/*
 * JsonServerCommChannelHolder.h
 *
 *  Created on: Oct 7, 2013
 *      Author: root
 */

#ifndef JSONSERVERCOMMCHANNELHOLDER_H_
#define JSONSERVERCOMMCHANNELHOLDER_H_

#include "jsonrpc/connectors/httpinterfaceserver.h"
#include <jsonrpc/connectors/httpinterfaceserverjsonp.h>
#include <jsonrpc/connectors/RawInterfaceServer.h>
#include <jsonrpc/connectors/SerialInterfaceServer.h>
#include <jsonrpc/connectors/WsInterfaceClient.h>
#include "TBS/Services/Json/JsonServices.h"
#include "TBS/Services/Services.h"
#include "jsonrpc/connectors/httpinterfaceclient.h"
#include "jsonrpc/connectors/WsInterfaceServer.h"

namespace TBS {
	namespace Services {

		class AJsonCommChannelHolder: public ICommChannelHolder {
			public:
				virtual ~AJsonCommChannelHolder() {
				}
				virtual jsonrpc::AbstractServerConnector & getInterface() = 0;
		};

		class JsonCommChannelHolder: public AJsonCommChannelHolder {

			public:

				JsonCommChannelHolder(const JsonServerParams & ch) :
						interface(ch) {

				}

				virtual jsonrpc::AbstractServerConnector & getInterface() {
					return interface;
				}
			private:
				jsonrpc::HttpInterfaceServer interface;
		};

		class JsonpCommChannelHolder: public AJsonCommChannelHolder {
			public:
				JsonpCommChannelHolder(const JsonServerParams & ch) :
						interface(ch) {
				}
				virtual jsonrpc::AbstractServerConnector & getInterface() {
					return interface;
				}
			private:
				jsonrpc::JsonpHttpInterfaceServer interface;
		};

		class WsCommChannelHolder: public AJsonCommChannelHolder {
			public:
				WsCommChannelHolder(const JsonServerParams & ch) :
						interface(ch) {
				}
				virtual jsonrpc::AbstractServerConnector & getInterface() {
					return interface;
				}
			private:
				jsonrpc::WsInterfaceServer interface;
		};

		class RawCommChannelHolder: public AJsonCommChannelHolder {
			public:
				RawCommChannelHolder(const JsonServerParams & ch) :
						interface(ch) {
				}
				virtual jsonrpc::AbstractServerConnector & getInterface() {
					return interface;
				}
			private:
				jsonrpc::RawInterfaceServer interface;
		};

		class SerialCommChannelHolder: public AJsonCommChannelHolder {
			public:
				SerialCommChannelHolder(const SerialParams & ch) :
						interface(ch) {
				}
				virtual jsonrpc::AbstractServerConnector & getInterface() {
					return interface;
				}
			private:
				jsonrpc::SerialInterfaceServer interface;
		};

		template<class TInterface, class TAdaptor>
		class JsonServerImpl: public IServer {

			public:

				JsonServerImpl(jsonrpc::AbstractServerConnector & serverinterface, typename TInterface::Ptr impl) :
						adaptor(serverinterface.handlers(), impl) {
				}
				virtual ~JsonServerImpl() {
				}
			private:
				TAdaptor adaptor;
		};


		jsonrpc::AbstractClientConnector::Ptr GEN_SERVICE_API createClientConnector(const std::string & name, const JsonClientParams & params);
		jsonrpc::AbstractClientConnector::Ptr GEN_SERVICE_API createClientConnector(const std::string & name, const SerialParams & params);

	} /* namespace Services */
} /* namespace TBS */
#endif /* JSONSERVERCOMMCHANNELHOLDER_H_ */
