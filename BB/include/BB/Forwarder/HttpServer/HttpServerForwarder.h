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
#include "BB/Services/WebUISvc_Json.h"

#include "BB/Forwarder/Filter.h"


namespace BB {


	class HttpServerForwarder : public IForwarder {
		public:
			HttpServerForwarder(int port, const IFilter::PtrList & filters, int oneSensorLimit);
			virtual ~HttpServerForwarder();

			virtual void forward(const SensorData & d);
			virtual void forward(const RuntimeStatus & d);
			virtual void forward(const Task & d);
			virtual void forward(const Notification & d);
		private:
			TBS::Services::IServer::Ptr sq;
			TBS::Services::IServer::Ptr sc;

			TBS::BB::WebUI::Json::Server::Ptr jsonServer;
			IForwarder::Ptr query;


	};

} /* namespace BB */
#endif /* HTTPSERVERFORWARDER_H_ */
