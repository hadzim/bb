/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    serverconnector.h
 * @date    31.12.2012
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef SERVERCONNECTOR_H_
#define SERVERCONNECTOR_H_

#include <string>
#include "rpcprotocolserver.h"
#include <Poco/SharedPtr.h>



namespace jsonrpc
{
    
	class AbstractServerConnector {

		public:
			typedef Poco::SharedPtr <AbstractServerConnector> Ptr;

			virtual bool StartListening() = 0;

			virtual bool StopListening() = 0;

			virtual void EmitSignal(std::string signal);
	};

	class ServerHandlerProvider {
		public:
			virtual ~ServerHandlerProvider(){}
			virtual RpcProtocolServer & GetHandler() = 0;
	};

    class SingleServerConnector : public ServerHandlerProvider, public AbstractServerConnector
    {
        public:

    		typedef Poco::SharedPtr <SingleServerConnector> Ptr;

            SingleServerConnector();
            virtual ~SingleServerConnector();

            /**
             * This method should send a response to the client in any way that is appropriate for the derived connector class.
             * @param response - the response that should be send to the client
             * @param addInfo - additional Info, that the Connector might need for responding.
             * @return returns true on success, false otherwise
             */
            //bool virtual SendResponse(const std::string& response, void* addInfo = NULL) = 0;

            /**
             * This method must be called, when a request is recognised. It will do everything else for you (including sending the response).
             * @param request - the request that has been recognised.
             * @param addInfo - additional Info, that the Connector might need for responding.
             */
            //bool OnRequest(const std::string& request, void* addInfo = NULL);

            std::string GetSpecification();

            void SetHandler(RpcProtocolServer & handler);

            virtual RpcProtocolServer & GetHandler();
        private:
            RpcProtocolServer * handler;
    };

} /* namespace jsonrpc */
#endif /* SERVERCONNECTOR_H_ */
