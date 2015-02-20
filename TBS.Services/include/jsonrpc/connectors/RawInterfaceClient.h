/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    httpclient.h
 * @date    02.01.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef RawWSHTTPCLIENTINT_H_
#define RawWSHTTPCLIENTINT_H_

#include "../clientconnector.h"
#include "../exception.h"

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>

#include <memory>
#include "TBS/Services/Json/JsonServices.h"


namespace jsonrpc
{


    class RawInterfaceClient : public AbstractClientConnector
    {
        public:
    		RawInterfaceClient(const  std::string & interfaceName, const TBS::Services::JsonClientParams & p);
            virtual ~RawInterfaceClient();

            virtual void SendMessage(const std::string& message, std::string& result);


        private:
            std::string interfaceName;

            TBS::Services::JsonClientParams p;

            typedef std::unique_ptr<Poco::Net::StreamSocket> SocketPtr;
            SocketPtr socket;
    };

} /* namespace jsonrpc */
#endif /* HTTPCLIENT_H_ */
