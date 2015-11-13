/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    httpserver.h
 * @date    31.12.2012
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef HttpInterfaceServerjsonpCONNECTOR_H_
#define HttpInterfaceServerjsonpCONNECTOR_H_


#include <jsonrpc/connectors/SharedHttpInterfaceServer.h>

namespace jsonrpc
{


    class GEN_SERVICE_API JsonpHttpInterfaceServer: public SharedHttpInterfaceServer
    {
        public:


    		JsonpHttpInterfaceServer(const TBS::Services::JsonServerParams & p);

    };

} /* namespace jsonrpc */
#endif /* HTTPSERVERCONNECTOR_H_ */
