/*
 * RemoteClientForwarder.h
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#ifndef RemoteClient_H_
#define RemoteClient_H_
#include <BB/Notification.h>
#include "Poco/SharedPtr.h"
#include "../Cache.h"
#include "../IForwarder.h"

#include "TBS/Log.h"

#include "Poco/Thread.h"

#include "Poco/Runnable.h"

namespace BB {

	class RemoteClientSettings {
		public:
			//load from cfg
			RemoteClientSettings();
			RemoteClientSettings(std::string url, int port, std::string query, std::string project);
		public:
			std::string url;
			int port;
			std::string query;
			std::string projectID;
	};

	class RemoteClient {
		public:
			RemoteClient(const RemoteClientSettings settings = RemoteClientSettings());

			void forward(const SensorData & data);
			void forward(const RuntimeStatus & data);
			void forward(const Notification & data);

			std::string remoteQuery(std::string action, std::string data);

			void sendImage(std::string path, std::string info);

		private:
			void sendToRemoteServer(const SensorData & m);
			void sendToRemoteServer(const RuntimeStatus & s);
			void sendToRemoteServer(const Notification & s);


			void sendCache();
			template<class DataTypeCache>
			void sendCacheImpl(DataTypeCache & cache) {
				typename DataTypeCache::Data data = cache.fetch();

				int cnt = 0;
				for (typename DataTypeCache::Data::iterator i = data.begin(); i != data.end(); i++) {
					try {
						sendToRemoteServer(*i);
						cnt++;
					} catch (Poco::Exception & e) {
						LWARNING("RemoteClient")<< "RemoteClientForwarder: send cached data failed: " << e.displayText() << LE;
							std::cerr << "send cache data: " << e.displayText() << std::endl;
							break;
						}
					}
				if (cnt) {
					cache.removeFirst(cnt);
				}
			}

			template<class DataType, class DataTypeCache>
			void forwardImpl(const DataType & d, DataTypeCache & cache) {
				std::cout << "filter data RC" << std::endl;
				bool ok = false;
				try {
					this->sendToRemoteServer(d);
					ok = true;
				} catch (Poco::Exception & e) {
					LWARNING("RemoteClient")<< "RemoteClientForwarder: send data failed: " << e.displayText() << LE;
						std::cerr << "send data: " << e.displayText() << std::endl;
						cache.store(d);
					}

				if (ok) {
					sendCache();
				}
			}
		private:
			typedef TypeCache<SensorData, SensorDataRW> SensorCache;
			typedef TypeCache<RuntimeStatus, StatusDataRW> StatusCache;
			typedef TypeCache<Notification, NotificationDataRW> NotificationCache;
			SensorCache sensorCache;
			StatusCache statusCache;
			NotificationCache notificationCache;

			RemoteClientSettings settings;

	};

} /* namespace BB */
#endif /* RemoteClientForwarder_H_ */
