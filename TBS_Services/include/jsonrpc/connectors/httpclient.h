/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    httpclient.h
 * @date    02.01.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef HTTPCLIENT_H_
#define HTTPCLIENT_H_

#include "../clientconnector.h"
#include "../exception.h"

#include <Poco/Net/HTTPClientSession.h>


namespace jsonrpc
{
    
    class HttpClient : public AbstractClientConnector
    {
        public:
            HttpClient(const std::string& host, int port);
            virtual ~HttpClient();

            virtual void SendMessage(const std::string& message, std::string& result);

            void SetUrl(const std::string& url);

        private:
            std::string url;
            int port;
            Poco::Net::HTTPClientSession client;

    };

} /* namespace jsonrpc */
#endif /* HTTPCLIENT_H_ */
