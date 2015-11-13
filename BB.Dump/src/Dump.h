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

#include "TBS/MQTT/Client.h"

namespace BB {


/*

	class Dump: public Poco::Util::ServerApplication {

		public:
			typedef Poco::SharedPtr<Dump> Ptr;
			Dump();
			virtual ~Dump();

		protected:

			int main(const std::vector<std::string>& args);
		private:
			void onData(TBS::BB::Services::Data::IDataDistributor::SensorDataReceivedArg & arg);
		private:


		private:

	};
*/

class Dump: public Poco::Util::ServerApplication {

	public:
		typedef Poco::SharedPtr<Dump> Ptr;
		Dump();
		virtual ~Dump();

	protected:

		int main(const std::vector<std::string>& args);
	private:
		void onMessage(TBS::MQTT::Message & m);
	private:


	private:
		TBS::MQTT::Client::Ptr c;
};

} /* namespace BB */
#endif /* HttpServer_H_ */
