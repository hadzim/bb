/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    clientconnector.h
 * @date    02.01.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef CLIENTCONNECTOR_H_
#define CLIENTCONNECTOR_H_

#include <string>
#include "exception.h"
#include <Poco/SharedPtr.h>
#include "TBS/Services/Types.h"

namespace jsonrpc
{
    class GEN_SERVICE_API AbstractClientConnector
    {
        public:
    		typedef Poco::SharedPtr <AbstractClientConnector> Ptr;
            virtual ~AbstractClientConnector();

            /**
             * This method should take the message and send it via the concrete connector.
             * The result of the request must be returned as string.
             */
            virtual std::string SendMessage(const std::string& message);

            virtual void SendMessage(const std::string& message, std::string& result) = 0;
    };
} /* namespace jsonrpc */
#endif /* CLIENTCONNECTOR_H_ */
