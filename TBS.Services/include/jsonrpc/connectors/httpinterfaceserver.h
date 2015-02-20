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

#include <memory>
#include "Poco/Net/HTTPServer.h"
#include "../ServiceHandlers.h"
#include "TBS/Services/Json/JsonServices.h"
#include "jsonrpc/AbstractServerConnector.h"

namespace jsonrpc
{

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
    		HttpInterfaceServer(const TBS::Services::JsonServerParams & p);
            virtual ~HttpInterfaceServer();

            virtual bool StartListening();
            virtual bool StopListening();


        private:
            TBS::Services::JsonServerParams p;
            std::unique_ptr<Poco::Net::HTTPServer> srv;
            //Poco::ThreadPool tp;

    };

} /* namespace jsonrpc */
#endif /* HTTPSERVERCONNECTOR_H_ */
