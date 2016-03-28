/*
 * main.cpp
 *
 *  Created on: Feb 23, 2015
 *      Author: dron
 */

#include "TBS/Services/Json/JsonServices.h"
#include "jsonrpc/connectors/HttpServer.h"
#include <Poco/Util/ServerApplication.h>
#include <BB/ServiceNotification.h>

class App : public Poco::Util::ServerApplication {
	protected:
		int main(const std::vector<std::string> & args){

			try {

				TBS::initLogs("www", 8, "/tmp/");

				LTRACE("www") << 1 << LE;
#ifdef TBS_TPRO
				TBS::Services::JsonServerParams p(80);
#else
				TBS::Services::JsonServerParams p(8180);
#endif

				p.MaxParallelThreads.set(10);

				LTRACE("www") << 2 << LE;

#ifdef TBS_TPRO
				p.addSpecialRequestHandler(
								new TBS::Services::FolderStreamRequestHandler("", "/home/tbs/data/")
						);


#else

				p.addSpecialRequestHandler(
								new TBS::Services::FolderStreamRequestHandler("", "data/")
						);



#endif


				p.addSpecialRequestHandler(
						new TBS::Services::RedirectRequestHandler("", "index.html")
				);

				LTRACE("www") << 3 << LE;

				jsonrpc::HttpServer server(p);

				LTRACE("www") << 4 << LE;

				server.StartListening();

				LTRACE("www") << 5 << LE;

				BB::ServiceNotification::serviceReady();

				this->waitForTerminationRequest();

				BB::ServiceNotification::serviceDisabled();


				server.StopListening();

			} catch (Poco::Exception & e){
				LERROR("www") << e.displayText() << LE;
			}
		}
};

POCO_SERVER_MAIN(App)

