/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    httpserver.h
 * @date    31.12.2012
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef HttpInterfaceServerCONNECTOR_H_
#define HttpInterfaceServerCONNECTOR_H_

#include "../serverconnector.h"
#include <memory>
#include "Poco/Net/HTTPServer.h"
#include "../ServiceHandlers.h"

namespace jsonrpc
{

	std::string md5Hash(std::string val);

	class HttpServerParams {
		public:
		int port;
		bool allowCrossDomain;
		bool isHttps;
		std::string privateKey;
		std::string certificate;
		bool isProtected;
		std::string username;
		std::string passwordMd5Hash;
		bool isDocFile;
		std::string docFile;

		explicit HttpServerParams(int port = 530) : port(port), allowCrossDomain(true), isHttps(false), isProtected(false), isDocFile(false){

		}
	};

    /**
     * This class provides an embedded HTTP Server, based on Mongoose, to handle incoming Requests and send HTTP 1.1
     * valid responses.
     * Note that this class will always send HTTP-Status 200, even though an JSON-RPC Error might have occurred. Please
     * always check for the JSON-RPC Error Header.
     */
    class HttpInterfaceServer: public AbstractServerConnector
    {
        public:

            /**
             * @brief HttpServer, constructor for the included HttpServer
             * @param port on which the server is listening
             * @param enableSpecification - defines if the specification is returned in case of a GET request
             * @param sslcert - defines the path to a SSL certificate, if this path is != "", then SSL/HTTPS is used with the given certificate.
             */
    		HttpInterfaceServer(const HttpServerParams & p);
            virtual ~HttpInterfaceServer();

            virtual bool StartListening();
            virtual bool StopListening();

            ServiceHandlers & handlers();

        private:
            HttpServerParams p;
            ServiceHandlers handlers_;
            std::auto_ptr<Poco::Net::HTTPServer> srv;

    };

} /* namespace jsonrpc */
#endif /* HTTPSERVERCONNECTOR_H_ */
