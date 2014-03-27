/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    httpclient.h
 * @date    02.01.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef DotNetClientH_H_
#define DotNetClientH_H_


#include <Poco/Net/HTTPClientSession.h>

#include <memory>
#include "json/value.h"
#include "TBS/Services/DotNet/DotNetServices.h"

namespace DotNet {



	class DotNetClient {
		public:
			//typedef std::auto_ptr<Poco::Net::DotNetClient> Ptr;

			DotNetClient(const TBS::Services::DotNetClientParams & params);
			Json::Value sendRequest(std::string url) ;

		private:
			TBS::Services::DotNetClientParams params;

			typedef std::auto_ptr<Poco::Net::HTTPClientSession> ClientPtr;
			ClientPtr client;

	};

} /* namespace DotNet */
#endif /* HTTPCLIENT_H_ */
