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
#include <Poco/SharedPtr.h>
#include "jsonrpc/ServiceHandlers.h"



namespace jsonrpc
{
    
	class AbstractServerConnector {
		private:
			ServiceHandlers::Ptr handlers_;
		public:
			typedef Poco::SharedPtr <AbstractServerConnector> Ptr;

			AbstractServerConnector(){
				this->handlers_ = ServiceHandlers::Ptr(new ServiceHandlers());
			}
			virtual ~AbstractServerConnector(){}

			virtual bool StartListening() = 0;

			virtual bool StopListening() = 0;

			virtual ServiceHandlers::Ptr handlers(){
				return this->handlers_;
			}

	};


} /* namespace jsonrpc */
#endif /* SERVERCONNECTOR_H_ */
