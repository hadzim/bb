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

namespace BB {




	class HttpServer: public Poco::Util::ServerApplication {

		public:
			typedef Poco::SharedPtr<HttpServer> Ptr;
			HttpServer();
			virtual ~HttpServer();

		protected:

			int main(const std::vector<std::string>& args);
		private:

		private:

	};

} /* namespace BB */
#endif /* HttpServer_H_ */
