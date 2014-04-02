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
#include "BB/Services/Sensor.h"
#include "BB/Services/SensorSvc_DBus.h"

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
			ForwarderApp(IForwarderFactory::Ptr factory);
			virtual ~ForwarderApp();

		protected:
			int main(const std::vector<std::string>& args);

		private:
			void onData(TBS::BB::Services::Sensor::IDataDistributor::SensorDataReceivedArg & arg);
		private:

			IForwarder::Ptr forwarder;
			IForwarderFactory::Ptr factory;

			TBS::BB::Services::Sensor::DBus::Client::Ptr observer;
			Poco::Mutex m;

	};

} /* namespace BB */
#endif /* ForwarderApp_H_ */
