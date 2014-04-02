/*
 * Device.h
 *
 *  Created on: Nov 9, 2012
 *      Author: root
 */

#ifndef TIMERNOTIFIER_H_
#define TIMERNOTIFIER_H_
#include "Poco/Mutex.h"
#include "Poco/Runnable.h"
#include <Poco/Thread.h>
#include "Poco/Event.h"

#include <list>
#include <algorithm>
#include "TimerImpl.h"
#include "Poco/Timestamp.h"
#include "Poco/SharedPtr.h"

#include "TBS/TBS.h"


namespace TBS {

		Poco::Timestamp::TimeVal monotonicTimestamp();

		class TBS_API TimerNotifier : private Poco::Runnable {
			public:

				typedef Poco::SharedPtr <TimerNotifier> Ptr;

				static TimerNotifier & instance();

				TimerNotifier(std::string name = "TimerNotifier");
				virtual ~TimerNotifier();

				void startTimer(TimerImpl::Ptr timer, int start, int periodic);
				void stopTimer(TimerImpl::Ptr timer);

			private:
				struct TimerHolder {

					typedef std::list <TimerHolder> List;

					TimerImpl::Ptr timer;

					Poco::Timestamp::TimeDiff wakeAfter;
					Poco::Timestamp::TimeVal startAt;

					int periodicInterval;

					bool invalidated;

					bool operator ==(const TimerHolder & h) const{
						return timer == h.timer;
					}

					TimerHolder();
				};

			private:

				void run();

				void initFirst();

				void remove(TimerHolder::List & from, const TimerHolder & th);
			private:


				bool shouldExit;
				//timer thread
				Poco::Thread t;
				Poco::Event evt;

				TimerHolder::List timers;
				TimerHolder::List timersToStart;
				TimerHolder::List timersToStop;
				Poco::Mutex tm;

				static Poco::Mutex m;
		};


} /* namespace TBS */
#endif /* DEVICE_H_ */
