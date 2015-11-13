/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    httpserver.h
 * @date    31.12.2012
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef SharedHttpInterfaceServerCONNECTOR_H_
#define SharedHttpInterfaceServerCONNECTOR_H_

#include <functional>
#include <memory>
#include "Poco/Net/HTTPServer.h"
#include "../ServiceHandlers.h"
#include "TBS/Services/Json/JsonServices.h"
#include "jsonrpc/AbstractServerConnector.h"
#include <Poco/Net/HTTPRequestHandlerFactory.h>

namespace jsonrpc
{

    class GEN_SERVICE_API SharedHttpInterfaceServer: public AbstractServerConnector
    {
        public:
    		typedef std::function<Poco::Net::HTTPRequestHandlerFactory::Ptr(ServiceHandlers::Ptr, const TBS::Services::JsonServerParams & p)> FactoryFnc;

    		SharedHttpInterfaceServer(const TBS::Services::JsonServerParams & p, FactoryFnc factoryCreator);
            virtual ~SharedHttpInterfaceServer();

            virtual bool StartListening();
            virtual bool StopListening();


        private:
            TBS::Services::JsonServerParams p;
            std::unique_ptr<Poco::Net::HTTPServer> srv;
            Poco::ThreadPool tp;

    };

} /* namespace jsonrpc */
#endif /* HTTPSERVERCONNECTOR_H_ */
