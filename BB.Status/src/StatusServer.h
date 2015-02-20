/*
 * HttpServer.h
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#ifndef HttpServer_H_
#define HttpServer_H_
#include "Poco/Util/ServerApplication.h"
#include "Poco/Timer.h"
#include "TBS/SafeTimer.h"
#include "BB/Sensor/ISensor.h"

#include "BB/Services/DataSvc_DBus.h"

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/PartHandler.h"
#include "Poco/Net/MessageHeader.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/CountingStream.h"
#include "Poco/NullStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Exception.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include <iostream>

#include "TBS/SimpleTimer.h"

using Poco::Net::ServerSocket;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPServerParams;
using Poco::Net::MessageHeader;
using Poco::Net::HTMLForm;
using Poco::Net::NameValueCollection;
using Poco::Util::ServerApplication;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::CountingInputStream;
using Poco::NullOutputStream;
using Poco::StreamCopier;

#include <deque>
#include "BB/RuntimeStatus.h"

namespace BB {

	class DataHolder {
			int holds;
		public:
			DataHolder(){
				holds = 200;
			}
			typedef Poco::SharedPtr<DataHolder> Ptr;

			std::deque<SensorData> lastData;
			std::deque<RuntimeStatus> lastStatus;

			Poco::Mutex m;

			void add(SensorData d){
				Poco::Mutex::ScopedLock l(m);
				lastData.push_front(d);

				while (lastData.size() > holds){
					lastData.pop_back();
				}
			}

			void add(RuntimeStatus d){
				Poco::Mutex::ScopedLock l(m);
				lastStatus.push_front(d);

				while (lastStatus.size() > holds){
					lastStatus.pop_back();
				}
			}


	};

	class StatusRequestHandler: public HTTPRequestHandler {
			DataHolder::Ptr dataHolder;
		public:
			StatusRequestHandler(DataHolder::Ptr dataHolder) :
					dataHolder(dataHolder) {
			}

			void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {

				response.setChunkedTransferEncoding(true);
				response.setContentType("text/html");

				std::ostream& ostr = response.send();

				ostr << "<html>\n"
						"<head>\n"
						"<title>BB status</title>\n"
						"</head>\n"
						"<body>\n"
						"<h1>BB status</h1>\n";

				ostr << "<h2>Last Sensor Data:</h2><p>\n";

				{
					Poco::Mutex::ScopedLock l(dataHolder->m);
					for (auto & data : dataHolder->lastData){
						ostr << "<div>" << "\n";
						ostr << data;
						ostr << "</div>" << "\n";
					}
				}

				ostr << "<h2>Last Runtime Status:</h2><p>\n";

				{
					Poco::Mutex::ScopedLock l(dataHolder->m);
					for (auto & data : dataHolder->lastStatus){
						ostr << "<div>" << "\n";
						ostr << data;
						ostr << "</div>" << "\n";
					}
				}

				ostr << "</body>\n";
				ostr << "</html>\n";
			}
	};

	class StatusRequestHandlerFactory: public HTTPRequestHandlerFactory {
		public:
			StatusRequestHandlerFactory(DataHolder::Ptr dataHolder) :
					dataHolder(dataHolder) {
			}

			HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request) {
				return new StatusRequestHandler(dataHolder);
			}
		private:
			DataHolder::Ptr dataHolder;
	};

	class StatusServer: public Poco::Util::ServerApplication {

		public:
			typedef Poco::SharedPtr<StatusServer> Ptr;
			StatusServer();
			virtual ~StatusServer();

		protected:

			int main(const std::vector<std::string>& args);
		private:
			void onData(TBS::BB::Services::Data::IDataDistributor::SensorDataReceivedArg & arg);
			void onStatus(TBS::BB::Services::Data::IDataDistributor::StatusReceivedArg & arg);

		private:
			DataHolder::Ptr dataHolder;
		private:
			//TBS::SimpleTimer t;

	};

} /* namespace BB */
#endif /* HttpServer_H_ */
