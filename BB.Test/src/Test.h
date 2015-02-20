/*
 * Test.h
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#ifndef Test_H_
#define Test_H_
#include "Poco/Util/ServerApplication.h"
#include "Poco/Timer.h"
#include "TBS/SafeTimer.h"
#include "BB/Sensor/ISensor.h"

#include "BB/Services/DataSvc_DBus.h"

namespace BB {




	class Test: public Poco::Util::ServerApplication {

		public:
			typedef Poco::SharedPtr<Test> Ptr;
			Test();
			virtual ~Test();

		protected:

			int main(const std::vector<std::string>& args);
		private:
			private:


		private:

	};

} /* namespace BB */
#endif /* Test_H_ */
