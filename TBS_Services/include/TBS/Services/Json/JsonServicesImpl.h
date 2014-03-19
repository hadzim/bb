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
#include "TBS/Services/Json/JsonServices.h"
#include "TBS/Services/Services.h"
#include "jsonrpc/connectors/httpinterfaceclient.h"

namespace TBS {
	namespace Services {

		jsonrpc::HttpServerParams convert(const JsonServerChannel & ch);
		jsonrpc::HttpClientParams convert(const JsonClientChannel & ch);

		class JsonCommChannelHolder: public ICommChannelHolder {

			public:

				JsonCommChannelHolder(const JsonServerChannel & ch) :
						interface(convert(ch)) {

				}

				jsonrpc::HttpInterfaceServer interface;
		};

		class JsonpCommChannelHolder: public ICommChannelHolder {

					public:

						JsonpCommChannelHolder(const JsonServerChannel & ch) :
								interface(convert(ch)) {

						}

						jsonrpc::JsonpHttpInterfaceServer interface;
				};

		template<class TInterface, class TAdaptor>
		class JsonServerImpl: public IServer {

			public:

				JsonServerImpl(jsonrpc::HttpInterfaceServer & interface, typename TInterface::Ptr impl) :
						adaptor(interface.handlers(), impl) {
				}
				JsonServerImpl(jsonrpc::JsonpHttpInterfaceServer & interface, typename TInterface::Ptr impl) :
						adaptor(interface.handlers(), impl) {
				}
				virtual ~JsonServerImpl() {
				}
			private:
				TAdaptor adaptor;
		};





	} /* namespace Services */
} /* namespace TBS */
#endif /* JSONSERVERCOMMCHANNELHOLDER_H_ */
