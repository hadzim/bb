/*
 * HttpServer.h
 *
 *  Created on: Feb 23, 2015
 *      Author: dron
 */

#ifndef HTTPSERVER_H_
#define HTTPSERVER_H_


#include <jsonrpc/connectors/SharedHttpInterfaceServer.h>


namespace jsonrpc
{

    class HttpServer: public SharedHttpInterfaceServer
    {
        public:

    		HttpServer(const TBS::Services::JsonServerParams & p);

        private:

    };

} /* namespace jsonrpc */

#endif /* HTTPSERVER_H_ */
