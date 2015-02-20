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

    /**
     * This class provides an embedded HTTP Server, based on Mongoose, to handle incoming Requests and send HTTP 1.1
     * valid responses.
     * Note that this class will always send HTTP-Status 200, even though an JSON-RPC Error might have occurred. Please
     * always check for the JSON-RPC Error Header.
     */
    class WsInterfaceServer: public AbstractServerConnector
    {
        public:


    		WsInterfaceServer(const TBS::Services::JsonServerParams & p);
            virtual ~WsInterfaceServer();

            virtual bool StartListening();
            virtual bool StopListening();


        private:
            TBS::Services::JsonServerParams p;
            std::unique_ptr<Poco::Net::HTTPServer> srv;

            std::vector <WebSocketWriter::Ptr> writers;

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
