/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    httpclient.h
 * @date    02.01.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef SERIALHTTPCLIENTINT_H_
#define SERIALHTTPCLIENTINT_H_


#include "../clientconnector.h"
#include "../exception.h"

#include <TBS/Services/Serial/SerialServices.h>
#include <TBS/Services/ITransportLayer.h>

#include <memory>

#include "serial/serial.h"



namespace jsonrpc
{


    class SerialInterfaceClient : public AbstractClientConnector
    {
        public:
    		SerialInterfaceClient(const  std::string & interfaceName, const TBS::Services::SerialParams & p);
            virtual ~SerialInterfaceClient();

            virtual void SendMessage(const std::string& message, std::string& result);


        private:
            std::string interfaceName;

            TBS::Services::SerialParams p;

            TBS::Services::ITransportClient::Ptr client;
            TBS::Services::ITransportLayer::Ptr layer;

    };

} /* namespace jsonrpc */
#endif /* HTTPCLIENT_H_ */
