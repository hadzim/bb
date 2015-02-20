/*
 * MotionSensor.cpp
 *
 *  Created on: Feb 3, 2013
 *      Author: root
 */

#include <Poco/Delegate.h>
#include <Poco/NumberFormatter.h>
#include "BB/Sensor/Board/MotionSensor.h"
#include <set>
#include "Poco/Glob.h"
#include "Poco/File.h"
#include <iostream>
#include "Poco/Path.h"
#include <fstream>
#include <sstream>
#include <stdio.h>
#include "BB/Configuration.h"

#include "TBS/Log.h"

#include "bonelib/gpio.h"

namespace BB {

	MotionSensorPins::MotionSensorPins(int slot) {
		if (slot < 1 || slot > 2) {
			throw Poco::Exception("Unknown motion slot");
		}

		int start = 41 - slot;

		in1 = start;
		in2 = start + 2;
		in3 = start + 4;

		out1 = 33;
		out2 = 35;
		out3 = 37;
	}

	MotionSensor::MotionSensor(int slot) :
			slot(slot), isConnected_(false), pins(slot) {

		BeagleBone::gpio::P8(pins.out1)->configure(BeagleBone::pin::OUT);
		BeagleBone::gpio::P8(pins.out2)->configure(BeagleBone::pin::OUT);
		BeagleBone::gpio::P8(pins.out3)->configure(BeagleBone::pin::OUT);

		BeagleBone::gpio::P8(pins.in1)->configure(BeagleBone::pin::IN);
		BeagleBone::gpio::P8(pins.in2)->configure(BeagleBone::pin::IN);
		BeagleBone::gpio::P8(pins.in3)->configure(BeagleBone::pin::IN);

		motionTimer.start(3000, 3000);
		motionTimer.Timer += Poco::delegate(this, &MotionSensor::onMotionTimer);

		connectionTimer.start(10000, 10000);
		connectionTimer.Timer += Poco::delegate(this, &MotionSensor::onConnectionTimer);
	}

	MotionSensor::~MotionSensor() {
		motionTimer.Timer -= Poco::delegate(this, &MotionSensor::onMotionTimer);
		motionTimer.stop();

		connectionTimer.Timer += Poco::delegate(this, &MotionSensor::onConnectionTimer);
		connectionTimer.stop();
	}

	bool MotionSensor::isConnected() {
		Poco::Mutex::ScopedLock l(m);
		return this->isConnected_;
	}
	void MotionSensor::setConnected(bool val) {
		Poco::Mutex::ScopedLock l(m);
		this->isConnected_ = val;
	}

	void MotionSensor::onMotionTimer(TBS::SimpleTimer::TimerArg &) {
		if (isConnected()) {
			BeagleBone::gpio* pin = BeagleBone::gpio::P8(pins.in1);
			int val = pin->get();

			{
				Poco::Mutex::ScopedLock l(m);
				if (ticks.isEmpty()) {
					ticks.set(ActiveAndTotal(0, 0));
				}
				ActiveAndTotal newval = ticks.val();
				newval.first += val;
				newval.second++;
				ticks.set(newval);
			}
		}
	}

	void MotionSensor::onConnectionTimer(TBS::SimpleTimer::TimerArg &) {
		try {
			BeagleBone::gpio* pinOut = BeagleBone::gpio::P8(pins.out3);
			BeagleBone::gpio* pinIn = BeagleBone::gpio::P8(pins.in3);

			unsigned char val = 0;
			for (int i = 0; i < 5; i++) {
				val = (val + 1) % 2;
				pinOut->set(val);
				Poco::Thread::sleep(3);

				std::cout << "MOTIONSENSOR: " << this->slot << " set: " << (int) val << "(" << pins.out3 << ")" << " get: " << (int) pinIn->get() << "("
						<< pins.in3 << ")" << std::endl;

				if (pinIn->get() != val) {
					throw Poco::Exception("Not connected");
				}

			}

			this->setConnected(true);
		} catch (...) {

			this->setConnected(false);

		}
	}

	MotionSensor::Requests MotionSensor::getRequests() {

		MotionSensor::Requests requests;

		int val = 0;
		{
			Poco::Mutex::ScopedLock l(m);
			if (ticks.isEmpty()) {
				return requests;
			} else {
				val = (ticks.val().first * 100) / (ticks.val().second);
			}
			ticks.reset();
		}

		std::string sensorID = "Motion-" + Poco::NumberFormatter::format(this->slot);

		SensorData s(SensorData::Motion, Configuration::getSensorName(SensorData::Motion, sensorID), sensorID, "", SensorData::localNow(),
				SensorData::Sensor_Ok, val, "");

		std::cout << s << std::endl;

		requests.push_back(s);

		return requests;

	}

	int MotionSensor::getPeriodInMs() {
		return 1000 * 60 * 2; // * 1000 * 60 * 5
	}

	std::string MotionSensor::getName() {
		return "BoardMotion";
	}

	MotionSensorBBB::MotionSensorBBB(int pin) :
			pin(pin), isConnected_(false), gpio(pin) {

		gpio.setDirection(exploringBB::GPIO::INPUT);

		this->setConnected(true);

		motionTimer.start(3000, 3000);
		motionTimer.Timer += Poco::delegate(this, &MotionSensorBBB::onMotionTimer);

	}

	MotionSensorBBB::~MotionSensorBBB() {
		motionTimer.Timer -= Poco::delegate(this, &MotionSensorBBB::onMotionTimer);
		motionTimer.stop();
	}

	bool MotionSensorBBB::isConnected() {
		Poco::Mutex::ScopedLock l(m);
		return this->isConnected_;
	}
	void MotionSensorBBB::setConnected(bool val) {
		Poco::Mutex::ScopedLock l(m);
		this->isConnected_ = val;
	}

	void MotionSensorBBB::onMotionTimer(TBS::SimpleTimer::TimerArg &) {
		if (isConnected()) {
			exploringBB::GPIO::VALUE val = gpio.getValue();

			{
				Poco::Mutex::ScopedLock l(m);
				if (ticks.isEmpty()) {
					ticks.set(ActiveAndTotal(0, 0));
				}
				ActiveAndTotal newval = ticks.val();
				newval.first += (int) val;
				newval.second++;
				ticks.set(newval);
			}
		}
	}

	MotionSensor::Requests MotionSensorBBB::getRequests() {

		MotionSensor::Requests requests;

		int val = 0;
		{
			Poco::Mutex::ScopedLock l(m);
			if (ticks.isEmpty()) {
				return requests;
			} else {
				val = (ticks.val().first * 100) / (ticks.val().second);
			}
			ticks.reset();
		}

		std::string sensorID = "Motion-" + Poco::NumberFormatter::format(this->pin);

		SensorData s(SensorData::Motion, Configuration::getSensorName(SensorData::Motion, sensorID), sensorID, "", SensorData::localNow(),
				SensorData::Sensor_Ok, val, "");

		std::cout << s << std::endl;

		requests.push_back(s);

		return requests;

	}

	int MotionSensorBBB::getPeriodInMs() {
		return 1000 * 60 * 2; // * 1000 * 60 * 5
	}

	std::string MotionSensorBBB::getName() {
		return "BoardMotionBBB";
	}

} /* namespace BB */
