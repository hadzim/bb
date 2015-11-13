/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    httpclient.h
 * @date    02.01.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef HTTPCLIENTINT_H_
#define HTTPCLIENTINT_H_

#include "../clientconnector.h"
#include "../exception.h"

#include <Poco/Net/HTTPClientSession.h>

#include <memory>
#include "TBS/Services/Json/JsonServices.h"


namespace jsonrpc
{


    class HttpInterfaceClient : public AbstractClientConnector
    {
        public:
    		HttpInterfaceClient(const  std::string & interfaceName, const TBS::Services::JsonClientParams & p);
            virtual ~HttpInterfaceClient();

            virtual void SendMessage(const std::string& message, std::string& result);


        private:
            std::string interfaceName;

            TBS::Services::JsonClientParams p;

            typedef std::unique_ptr<Poco::Net::HTTPClientSession> ClientPtr;
            ClientPtr client;

    };

} /* namespace jsonrpc */
#endif /* HTTPCLIENT_H_ */
