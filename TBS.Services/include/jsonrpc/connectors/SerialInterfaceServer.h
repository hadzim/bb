/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    httpserver.h
 * @date    31.12.2012
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef SerialInterfaceServerCONNECTOR_H_
#define SerialInterfaceServerCONNECTOR_H_

#include <Poco/Thread.h>
#include <TBS/Services/ITransportLayer.h>
#include <TBS/Services/Serial/SerialServices.h>
#include <memory>
#include "../ServiceHandlers.h"
#include "TBS/Services/Json/JsonServices.h"
#include "jsonrpc/AbstractServerConnector.h"

#include "Poco/Runnable.h"

namespace jsonrpc
{

    class GEN_SERVICE_API SerialInterfaceServer: public AbstractServerConnector, public Poco::Runnable
    {
        public:
    		SerialInterfaceServer(const TBS::Services::SerialParams & p);
            virtual ~SerialInterfaceServer();

            virtual bool StartListening();
            virtual bool StopListening();
        private:
            void run();
            void onRequest(TBS::Services::ITransportServer::Message & msg);

        private:
            //serial::Serial serial;
            TBS::Services::SerialParams p;

            bool listening;
            bool stopped;

            Poco::Thread t;

            TBS::Services::ITransportServer::Ptr server;
		    TBS::Services::ITransportLayer::Ptr layer;




    };

} /* namespace jsonrpc */
#endif /* HTTPSERVERCONNECTOR_H_ */
