/*
 * RemoteClientForwarder.h
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#ifndef RemoteClientForwarder_H_
#define RemoteClientForwarder_H_
#include "Poco/SharedPtr.h"
#include "../Cache.h"
#include "../IForwarder.h"

namespace BB {

	class RemoteClientForwarder: public IForwarder {
/*
			class Cfg : public Configuration {
				public:
					Cfg();
					std::string getHost();
					std::string getRequest();
				private:
					Poco::AutoPtr<Poco::Util::ConfigurationView> webserver;
			};
*/
		public:
			typedef Poco::SharedPtr<RemoteClientForwarder> Ptr;
			RemoteClientForwarder(std::string ip, int port, std::string requestUrl, std::string projetID);
			virtual ~RemoteClientForwarder();

			void forward(const SensorData & data);
		private:
			void sendToRemoteServer(const SensorData & m);
			void sendCache();
		private:
			Cache cache;

			std::string ip;
			int port;
			std::string requestUrl;
			std::string projectID;


	};

} /* namespace BB */
#endif /* RemoteClientForwarder_H_ */
