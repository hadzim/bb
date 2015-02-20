/*
 * Test.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#include "Test.h"
#include <iostream>
#include <Poco/Delegate.h>
#include "Poco/Thread.h"

#include "BB/Services/Data.h"
#include "BB/Services/DataSvc_DBus.h"
#include "BB/Sensor/SensorDataHelpers.h"
#include "BB/ServiceNotification.h"
#include "BB/Configuration.h"
#include <iostream>

#include "bonelib/gpio.h"


namespace BB {


Test::Test() {

}

Test::~Test() {

}

int Test::main(const std::vector<std::string>& args) {

/*
 * MOTION
 */

	BeagleBone::gpio* pin = BeagleBone::gpio::P8(46);
	pin->configure(BeagleBone::pin::IN);

	while (true){
		std::cout << (int)pin->get() << std::endl;
		Poco::Thread::sleep(200);
	}


	/*
	 * BUZZER
	 * */
/*
	BeagleBone::gpio* pin = BeagleBone::gpio::P8(45);
	pin->configure(BeagleBone::pin::OUT);

	int val = 0;

	while (true){
		int cval = ((val++)% 2) ? 1 : 0;
		pin->set(cval);
		std::cout << cval << std::endl;
		Poco::Thread::sleep(2000);
	}
*/

	/*std::cout << "main Test.start" << std::endl;

	{
		BB::Configuration::setSensorProperty(
				BB::SensorData::Temperature, "28-000001b4393b", BB::Configuration::NameProperty, "Inside"
		);
	}

	std::cout << "main Test.stop" << std::endl;
	 */



	return EXIT_OK;
}

} /* namespace BB */
