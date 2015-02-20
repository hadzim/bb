/*
 * ForwarderApp.h
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#ifndef ForwarderApp_H_
#define ForwarderApp_H_
#include "Poco/Util/ServerApplication.h"
#include "Poco/Timer.h"
#include "TBS/SafeTimer.h"
#include "BB/Forwarder/IForwarder.h"
#include <vector>
#include "BB/Services/Data.h"
#include "BB/Services/DataSvc_DBus.h"

namespace BB {

	class IForwarderFactory {
		public:
			typedef Poco::SharedPtr<IForwarderFactory> Ptr;

			virtual ~IForwarderFactory() {

			}
			virtual IForwarder::Ptr createForwarder() = 0;
	};

	class ForwarderApp: public Poco::Util::ServerApplication {

		public:
			typedef Poco::SharedPtr<ForwarderApp> Ptr;
			ForwarderApp(std::string name, IForwarderFactory::Ptr factory);
			ForwarderApp(std::string name, IForwarderFactory::Ptr factory1, IForwarderFactory::Ptr factory2);
			virtual ~ForwarderApp();

		protected:
			int main(const std::vector<std::string>& args);

		private:
			void onData(TBS::BB::Services::Data::IDataDistributor::SensorDataReceivedArg & arg);
			void onStatus(TBS::BB::Services::Data::IDataDistributor::StatusReceivedArg & arg);
			void onTask(TBS::BB::Services::Data::IDataDistributor::TaskReceivedArg & arg);
			void onNtf(TBS::BB::Services::Data::IDataDistributor::NotificationReceivedArg & arg);
		private:
			std::string name;
			IForwarder::Ptr forwarder;
			IForwarder::Ptr forwarder2;
			IForwarderFactory::Ptr factory;
			IForwarderFactory::Ptr factory2;

			TBS::BB::Services::Data::DBus::Client::Ptr observer;
			Poco::Mutex m;

			RuntimeStatus cummulativeStatus;
	};

} /* namespace BB */
#endif /* ForwarderApp_H_ */
