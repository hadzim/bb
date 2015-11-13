/*
 * TimerNotifier.cpp
 *
 *  Created on: Nov 9, 2014
 *      Author: root
 */

#include "TBS/TimerNotifier.h"

#include <Poco/String.h>
#include <TBS/Nullable.h>

#include <iostream>

#include "TBS/Log.h"

namespace TBS {

	Poco::Timestamp::TimeVal monotonicTimestamp() {

#if defined(POCO_OS_FAMILY_WINDOWS)

		FILETIME ft;
#if defined(_WIN32_WCE)
		GetSystemTimeAsFileTimeWithMillisecondResolution(&ft);
#else
		GetSystemTimeAsFileTime(&ft);
#endif

		ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
		epoch.LowPart = 0xD53E8000;
		epoch.HighPart = 0x019DB1DE;

		ULARGE_INTEGER ts;
		ts.LowPart = ft.dwLowDateTime;
		ts.HighPart = ft.dwHighDateTime;
		ts.QuadPart -= epoch.QuadPart;
		return ts.QuadPart / 10;

#else
		struct timespec ts;
		if (clock_gettime(CLOCK_MONOTONIC, &ts))
		throw Poco::SystemException("cannot get time of day");
		return Poco::Timestamp::TimeVal(ts.tv_sec)*Poco::Timestamp::resolution() + ts.tv_nsec/1000;
#endif
	}

	Poco::Mutex TimerNotifier::m;

	TimerNotifier::TimerHolder::TimerHolder() : wakeAfter(0), startAt(0), periodicInterval(0), invalidated(false){
							startAt = monotonicTimestamp();
						}

	TimerNotifier::TimerNotifier(std::string name) :
			shouldExit(false), t(name) {
		t.start(*this);
	}

	TimerNotifier::~TimerNotifier() {
		shouldExit = true;
		evt.set();
		t.join();
	}

	TimerNotifier & TimerNotifier::instance() {
		Poco::Mutex::ScopedLock l(m);
		static TimerNotifier ntf;
		return ntf;
	}

	void TimerNotifier::startTimer(TimerImpl::Ptr timer, int start, int periodic) {
		{
			Poco::Mutex::ScopedLock l(tm);

			if (start < 0) {
				throw Poco::Exception("Cannot start timer with negative time");
			}
			if (periodic < 0) {
				throw Poco::Exception("Cannot apply periodic interval with negative time");
			}

			TimerHolder th;
			th.timer = timer;
			th.periodicInterval = periodic;
			th.wakeAfter = start * 1000;
			this->remove(this->timersToStop, th);
			this->timersToStart.push_back(th);
			evt.set();
		}
	}
	void TimerNotifier::stopTimer(TimerImpl::Ptr timer) {
		{
			Poco::Mutex::ScopedLock l(tm);
			TimerHolder th;
			th.timer = timer;
			this->remove(this->timersToStart, th);
			this->timersToStop.push_back(th);
			evt.set();
		}

	}

	void TimerNotifier::remove(TimerHolder::List & from, const TimerHolder & th) {
		TimerHolder::List::iterator e = std::find(from.begin(), from.end(), th);
		if (e != from.end()) {
			from.erase(e);
		}
	}

	void TimerNotifier::run() {

		TBS::threadDebug();

		const int DefaultWait = 10 * 1000;
		int nextWait = DefaultWait;
		while (!shouldExit) {
			if (!evt.tryWait(nextWait)) {
				evt.reset();
			}

			if (shouldExit)
				break;

			{
				Poco::Mutex::ScopedLock l(tm);
				for (TimerHolder::List::iterator it = this->timersToStart.begin(); it != this->timersToStart.end(); ++it) {
					this->remove(this->timers, *it);
					this->timers.push_back(*it);
				}
				this->timersToStart.clear();

				for (TimerHolder::List::iterator it = this->timersToStop.begin(); it != this->timersToStop.end(); ++it) {
					this->remove(this->timers, *it);
				}
				this->timersToStop.clear();
			}

			//raise events
			Poco::Timestamp::TimeVal now = monotonicTimestamp();
			for (TimerHolder::List::iterator it = this->timers.begin(); it != this->timers.end(); ++it) {
				Poco::Timestamp::TimeDiff diff = now - it->startAt;
				if (diff > it->wakeAfter) {

					if (it->periodicInterval > 0) {
						it->startAt = now;
						it->wakeAfter = it->periodicInterval * 1000;
					} else {
						it->invalidated = true;
					}
					try {
						//raise event
						it->timer->Timer(this, diff);

					} catch (Poco::Exception & e){
						LCRITICAL("Timer") << "Unhandled timer exception: " << e.displayText() << LE;
					} catch (std::exception & e){
						LCRITICAL("Timer") << "Unhandled timer exception: " << e.what() << LE;
					} catch (...){
						LCRITICAL("Timer") << "Unhandled timer exception: " << "???" << LE;
					}
				}
			}

			//set next sleep
			TimerHolder::List listCpy;
			Nullable<Poco::Timestamp::TimeDiff> minSleep;
			for (TimerHolder::List::iterator it = this->timers.begin(); it != this->timers.end(); ++it) {
				if (it->invalidated) {
					continue;
				}
				listCpy.push_back(*it);
				Poco::Timestamp::TimeDiff next = it->wakeAfter - (now - it->startAt);
				if (minSleep.isEmpty() || minSleep.ref() > next) {
					minSleep.set(next);
				}
			}

			//remove invalidated
			this->timers = listCpy;

			//assign sleep interval
			if (minSleep.isEmpty()) {
				nextWait = DefaultWait;
			} else {
				nextWait = minSleep.ref() / 1000;
			}
			//std::cout << "next sleep: " << nextWait << std::endl;

		}
	}

} /* namespace TBS */
