/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    httpserver.h
 * @date    31.12.2012
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef WSHttpInterfaceServerCONNECTOR_H_
#define WSHttpInterfaceServerCONNECTOR_H_

#include <jsonrpc/connectors/SharedHttpInterfaceServer.h>
#include <memory>
#include "Poco/Net/HTTPServer.h"
#include "../ServiceHandlers.h"
#include "TBS/Services/Json/JsonServices.h"


#include <memory>
#include "jsonrpc/AbstractServerConnector.h"

#ifndef OLDPOCO13

#include "Poco/Net/WebSocket.h"

namespace jsonrpc
{

	class WebSocketWriter {
		public:
			typedef Poco::SharedPtr <WebSocketWriter> Ptr;
			typedef std::weak_ptr <Poco::Net::WebSocket> WSWeakPtr;
			typedef std::shared_ptr <Poco::Net::WebSocket> WSStrongPtr;

			WebSocketWriter(WSWeakPtr ws) : ws(ws){

			}
		private:
			WSWeakPtr ws;
	};

    class GEN_SERVICE_API WsInterfaceServer: public SharedHttpInterfaceServer
    {
        public:

    		WsInterfaceServer(const TBS::Services::JsonServerParams & p);

    };

} /* namespace jsonrpc */

#else
namespace jsonrpc
{
	class WsInterfaceServer: public AbstractServerConnector
    {
        public:

    		WsInterfaceServer(const TBS::Services::JsonServerParams & p){}
            virtual ~WsInterfaceServer(){}

            virtual bool StartListening(){}
            virtual bool StopListening(){}

            ServiceHandlers & handlers(){
            	throw Poco::Exception("not implemented");
            }

    };
}
#endif

#endif /* HTTPSERVERCONNECTOR_H_ */
