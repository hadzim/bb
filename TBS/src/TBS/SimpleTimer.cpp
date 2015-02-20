/*
 * SimpleTimer.cpp
 *
 *  Created on: 25.3.2014
 *      Author: JV
 */

#include "TBS/SimpleTimer.h"
#include <Poco/Delegate.h>
#include "TBS/TimerNotifier.h"

namespace TBS {

	SimpleTimer::SimpleTimer() : isRunning_(false), isOneShot(false) {
		timer = new TimerImpl();

	}

	SimpleTimer::SimpleTimer(TimerNotifier::Ptr tntf) : isRunning_(false), isOneShot(false)  {
		timer = new TimerImpl();
		notifier = tntf;
	}

	SimpleTimer::~SimpleTimer() {
		this->Timer.clear();
		this->stop();
		timer = NULL;
	}

	void SimpleTimer::start(long startInterval, long periodicInterval) {
		isRunning_ = true;
		isOneShot = periodicInterval == 0;

		timer->Timer -= Poco::delegate(this, &SimpleTimer::onTimer);
		timer->Timer += Poco::delegate(this, &SimpleTimer::onTimer);

		if (notifier.isNull()) {
			TimerNotifier::instance().startTimer(timer, startInterval, periodicInterval);
		} else {
			notifier->startTimer(timer, startInterval, periodicInterval);
		}

	}
	void SimpleTimer::stop() {
		isRunning_ = false;
		timer->Timer -= Poco::delegate(this, &SimpleTimer::onTimer);
		if (notifier.isNull()) {
			TimerNotifier::instance().stopTimer(timer);
		} else {
			notifier->stopTimer(timer);
		}
	}

	void SimpleTimer::onTimer(TimerArg & a) {

		this->Timer(this, a);
		if (isOneShot){
			isRunning_ = false;
		}
	}

	bool SimpleTimer::isRunning() const{
		return this->isRunning_;
	}

} /* namespace TBS */
