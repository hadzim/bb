/*
 * BoardMotionNode.cpp
 *
 *  Created on: Dec 10, 2015
 *      Author: dron
 */
#include <BB/Node/Board/BoardMotionNode.h>
#include <Poco/Delegate.h>

namespace BB {

	static BB::Node::Settings boardMotionAdditional(){

		BB::Node::Settings s({
			BB::Node::Setting("gpinNumber", "text", BB::Node::Setting::Value(26)),
		});

		return s;
	}

	BoardMotionNode::BoardMotionNode(std::string uid) : MotionNode(uid, 10*1000, boardMotionAdditional()) {
		motionTimer.start(1000, 1000);
		motionTimer.Timer += Poco::delegate(this, &BoardMotionNode::onMotionTimer);

	}

	BoardMotionNode::~BoardMotionNode() {
		motionTimer.stop();
		motionTimer.Timer -= Poco::delegate(this, &BoardMotionNode::onMotionTimer);

	}

	void BoardMotionNode::onMotionTimer(TBS::SimpleTimer::TimerArg &) {
		std::cout << "motion timer pin is set?" << (pin.isSet() ? 1 : 0) << std::endl;
		if (pin.isSet()){
			std::cout << "motion timer" << std::endl;
			exploringBB::GPIO gpio(pin.val());
			gpio.setDirection(exploringBB::GPIO::INPUT);
			int val = gpio.getValue() == exploringBB::GPIO::HIGH;

			Poco::Mutex::ScopedLock l(m);
			if (ticks.isEmpty()) {
				ticks.set(ActiveAndTotal(0, 0));
			}

			ActiveAndTotal newval = ticks.val();
			newval.first += val;
			newval.second++;
			ticks.set(newval);

			std::cout << "motion timer: " << val << std::endl;
		}
	}

	BB::Node::Data BoardMotionNode::readOne(const BB::Node::Info & info, const BB::Node::Sensor & sensor) {
		int val = 0;
		{
			Poco::Mutex::ScopedLock l(m);
			if (ticks.isSet()) {
				val = (ticks.val().first * 100) / (ticks.val().second);
			}
			ticks.reset();
		}

		BB::Node::Data data(val, BB::Node::localNow());
		return data;
	}

	void BoardMotionNode::afterRead(Node::Data & data, const Node::Info & info, const Node::Sensor & sensor){
		std::cout << "afterRead: " << std::endl;

		auto settings = getSettings();

		int pinNumber = false;
		if (Node::isFilled(settings.at("gpinNumber"), pinNumber) && pinNumber){
			std::cout << "afterRead: " << pinNumber << std::endl;
			pin.set(pinNumber);
		}

		MotionNode::afterRead(data, info, sensor);
	}



} /* namespace BB */
