/*
 * SerialCollectorService.cpp
 *
 *  Created on: 18.3.2014
 *      Author: JV
 */

#include "SerialCollectorService.h"
#include <iostream>
#include <Poco/Delegate.h>
#include <Poco/Util/AbstractConfiguration.h>
#include <Poco/Util/XMLConfiguration.h>
#include "BB/Services/DataSvc_DBus.h"

#include "BB/ServiceNotification.h"

#include "BB/Serial/lib/serial.h"
#include "BB/Serial/SerialMessage.h"

#include "bonelib/pinmux.h"
#include "bonelib/gpio.h"

#include "TBS/Log.h"

static void switch_pin(BeagleBone::pin* p, BeagleBone::pin_fct* f) {
	printf("\nSwitching pin %s to %s function...\n", p->get_name(), f->get_name());
	p->xport(f);
	system("cat /sys/kernel/debug/omap_mux/ecap0_in_pwm0_out");
}

namespace BB {

	SerialBg::SerialBg() :
			finished(false) {

	}
	SerialBg::~SerialBg() {
	}

	void SerialBg::setFinished() {
		finished = true;
	}

	void SerialBg::run() {
		try {

			TBS::initLogs("SerialCollector", 6, "/tmp/");


			TBS::BB::Services::Data::DBus::Client client;

#ifdef _WIN32
			serial::Serial serial("COM2");
#else
/*
			BeagleBone::pin *P9_21 = BeagleBone::pin::P9(21);
			BeagleBone::pin *P9_22 = BeagleBone::pin::P9(22);
			printf("Pin %s is %s...\n", P9_21->get_name(), P9_21->get_fct()->get_name());
			printf("Pin %s is %s...\n", P9_22->get_name(), P9_22->get_fct()->get_name());
			//  system("cat /sys/kernel/debug/omap_mux/ecap0_in_pwm0_out");
			switch_pin(P9_21, BeagleBone::pin_fct::uart2_txd);
			switch_pin(P9_22, BeagleBone::pin_fct::uart2_rxd);

			printf("Pin %s is %s...\n", P9_21->get_name(), P9_21->get_fct()->get_name());
			printf("Pin %s is %s...\n", P9_22->get_name(), P9_22->get_fct()->get_name());
*/
			serial::Serial serial("/dev/ttyO4");

#endif

			//TBS::BB::Services::Data::DBus::Client::Ptr client = new TBS::BB::Services::Data::DBus::Client();

			while (!finished) {
				int a = serial.available();
				if (a > 0) {
					std::cout << "available: " << a << std::endl;
					std::vector<unsigned char> buffer;
					serial.read(buffer, a);

					for (std::size_t i = 0; i < buffer.size(); i++) {
						std::cout << buffer[i] << std::endl;
					}

					try {
						SerialMessage message(buffer);
						std::cout << "SerialMessage: " << message << std::endl;
						LINFO("Serial") << "SerialMessage: " << message << LE;
						SensorData data = message.createSensorData();
						BB::SensorDataHelpers::sendData(client.DataCollector(), data);
					} catch (Poco::Exception & e) {
						LERROR("Serial") << "SerialMessage error: " << e.displayText() << LE;
						std::cout << "error: " << e.displayText() << std::endl;
					} catch (std::exception & e) {
						LERROR("Serial") << "SerialMessage error: " << e.what() << LE;
						std::cout << "error std: " << e.what() << std::endl;
					}
				}
				Poco::Thread::sleep(50);

			}
		} catch (Poco::Exception & e) {
			LCRITICAL("Serial") << "SerialMessage error: " << e.displayText() << LE;
			std::cout << "Serial BG exc: " << e.displayText() << std::endl;
		} catch (std::exception & e) {
			LCRITICAL("Serial") << "SerialMessage error: " << e.what() << LE;
			std::cout << "Serial BG exc: " << e.what() << std::endl;
		} catch (...) {
			LCRITICAL("Serial") << "SerialMessage error: xxx" << LE;
			std::cout << "Serial BG exc: ???" << std::endl;
		}

	}

	SerialCollectorService::SerialCollectorService() {

	}

	SerialCollectorService::~SerialCollectorService() {
		t.join();
	}

	int SerialCollectorService::main(const std::vector<std::string>& args) {
		try {
			std::cout << "Serial Collector starts" << std::endl;


			{

				t.start(serialBg);

				BB::ServiceNotification::serviceReady();

				this->waitForTerminationRequest();

				BB::ServiceNotification::serviceDisabled();
				//Poco::Thread::sleep(4000);
			}

			std::cout << "Serial Collector finished" << std::endl;
		} catch (Poco::Exception & e) {
			LCRITICAL("Serial") << "SerialMessage error: " << e.displayText() << LE;
			std::cout << "Serial BG exc: " << e.displayText() << std::endl;
		} catch (std::exception & e) {
			LCRITICAL("Serial") << "SerialMessage error: " << e.what() << LE;
			std::cout << "Serial BG exc: " << e.what() << std::endl;
		} catch (...) {
			LCRITICAL("Serial") << "SerialMessage error: xxx" << LE;
			std::cout << "Serial BG exc: ???" << std::endl;
		}


		serialBg.setFinished();

		return EXIT_OK;
	}

} /* namespace BB */
