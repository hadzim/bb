/*
 * HttpServerForwarder.h
 *
 *  Created on: 2.4.2014
 *      Author: JV
 */

#ifndef HTTPSERVERFORWARDER_H_
#define HTTPSERVERFORWARDER_H_
#include "BB/Forwarder/IForwarder.h"
#include <TBS/Services/Services.h>
#include "BB/Services/WebUISvc_Jsonp.h"


namespace BB {


	class HttpServerForwarder : public IForwarder {
		public:
			HttpServerForwarder(int port);
			virtual ~HttpServerForwarder();

			virtual void forward(const SensorData & d);
		private:
			TBS::Services::IServer::Ptr sq;
			TBS::Services::IServer::Ptr sc;

			TBS::BB::WebUI::Json::Server::Ptr jsonServer;
			IForwarder::Ptr query;
	};

} /* namespace BB */
#endif /* HTTPSERVERFORWARDER_H_ */
