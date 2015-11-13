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

#include <jsonrpc/connectors/SharedHttpInterfaceServer.h>


namespace jsonrpc
{

    class GEN_SERVICE_API HttpInterfaceServer: public SharedHttpInterfaceServer
    {
        public:

    		HttpInterfaceServer(const TBS::Services::JsonServerParams & p);

        private:

    };

} /* namespace jsonrpc */
#endif /* HTTPSERVERCONNECTOR_H_ */
