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
#include "BB/Services/SensorSvc_DBus.h"

#include "BB/ServiceNotification.h"

#include "BB/Serial/lib/serial.h"
#include "BB/Serial/SerialMessage.h"

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
			serial::Serial serial("COM2");

			TBS::BB::Services::Sensor::DBus::Client::Ptr client = new TBS::BB::Services::Sensor::DBus::Client();

			while (!finished) {
				int a = serial.available();
				if (a > 0) {
					std::cout << "available: " << a << std::endl;
					std::vector<unsigned char> buffer;
					serial.read(buffer, a);

					try {
						SerialMessage message(buffer);
						std::cout << "SerialMessage: " << message << std::endl;

						SensorData data = message.createSensorData();
						BB::SensorDataHelpers::sendData(client->DataCollector(), data);
					} catch (Poco::Exception & e) {
						std::cout << "error: " << e.displayText() << std::endl;
					} catch (std::exception & e) {
						std::cout << "error std: " << e.what() << std::endl;
					}
				}
				Poco::Thread::sleep(50);

			}
		} catch (Poco::Exception & e) {
			std::cout << "Serial BG exc: " << e.displayText() << std::endl;
		} catch (std::exception & e) {
			std::cout << "Serial BG exc: " << e.what() << std::endl;
		} catch (...) {
			std::cout << "Serial BG exc: ???"  << std::endl;
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
		} catch (std::exception & e) {
			std::cout << "exc: " << e.what() << std::endl;
		} catch (...) {

		}

		serialBg.setFinished();

		return EXIT_OK;
	}

} /* namespace BB */
