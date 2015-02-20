/*
 * SerialCollectorService.h
 *
 *  Created on: 18.3.2014
 *      Author: JV
 */

#ifndef SERIALCOLLECTORSERVICE_H_
#define SERIALCOLLECTORSERVICE_H_

#include "Poco/Util/ServerApplication.h"
#include "BB/Configuration.h"
#include "BB/Services/Data.h"

#include "BB/Sensor/SensorData.h"
#include "BB/Sensor/SensorDataHelpers.h"

#include <iostream>
#include <Poco/SharedPtr.h>

namespace BB {

	class SerialBg : public Poco::Runnable {
		public:
			SerialBg();
			~SerialBg();

			virtual void run();
			void setFinished();
		private:
			bool finished;
	};

class SerialCollectorService: public Poco::Util::ServerApplication {

public:

	typedef Poco::SharedPtr<SerialCollectorService> Ptr;
	SerialCollectorService();
	virtual ~SerialCollectorService();
protected:

	int main(const std::vector<std::string>& args);

private:
	SerialBg serialBg;
	Poco::Thread t;

};

}

#endif /* COLLECTORSERVICE_H_ */
