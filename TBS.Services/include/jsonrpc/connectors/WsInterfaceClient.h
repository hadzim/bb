/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    httpclient.h
 * @date    02.01.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef WSHTTPCLIENTINT_H_
#define WSHTTPCLIENTINT_H_


#include "../clientconnector.h"
#include "../exception.h"

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>

#include <memory>
#include "TBS/Services/Json/JsonServices.h"

#ifndef OLDPOCO13

#include "Poco/Net/WebSocket.h"


namespace jsonrpc
{


    class WsInterfaceClient : public AbstractClientConnector
    {
        public:
    		WsInterfaceClient(const  std::string & interfaceName, const TBS::Services::JsonClientParams & p);
            virtual ~WsInterfaceClient();

            virtual void SendMessage(const std::string& message, std::string& result);


        private:
            std::string interfaceName;

            TBS::Services::JsonClientParams p;

            typedef std::unique_ptr<Poco::Net::HTTPClientSession> ClientPtr;
            ClientPtr client;

            typedef std::unique_ptr<Poco::Net::WebSocket> WsPtr;
            WsPtr ws;

            typedef std::unique_ptr<Poco::Net::HTTPRequest> RequestPtr;


            RequestPtr request;
            Poco::Net::HTTPResponse response;

    };

} /* namespace jsonrpc */

#else

namespace jsonrpc
{


    class WsInterfaceClient : public AbstractClientConnector
    {
        public:
    		WsInterfaceClient(const  std::string & interfaceName, const TBS::Services::JsonClientParams & p){}
            virtual ~WsInterfaceClient(){}

            virtual void SendMessage(const std::string& message, std::string& result){

            }


        private:

    };

} /* namespace jsonrpc */

#endif

#endif /* HTTPCLIENT_H_ */

