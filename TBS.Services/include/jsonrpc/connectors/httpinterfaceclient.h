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


namespace jsonrpc
{
    
	class HttpClientParams {
		public:
			std::string host;
			int port;
			bool allowCrossDomain;
			bool isHttps;
			bool isProtected;
			std::string username;
			std::string password;
			HttpClientParams();
	};

    class HttpInterfaceClient : public AbstractClientConnector
    {
        public:
    		HttpInterfaceClient(const  std::string & interfaceName, const HttpClientParams & p);
            virtual ~HttpInterfaceClient();

            virtual void SendMessage(const std::string& message, std::string& result);

            void SetUrl(const std::string& url);

        private:
            std::string interfaceName;

            HttpClientParams p;

            typedef std::auto_ptr<Poco::Net::HTTPClientSession> ClientPtr;
            ClientPtr client;

    };

} /* namespace jsonrpc */
#endif /* HTTPCLIENT_H_ */
