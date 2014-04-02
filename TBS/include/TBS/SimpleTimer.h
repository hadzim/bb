/*
 * SimpleTimer.h
 *
 *  Created on: 25.3.2014
 *      Author: JV
 */

#ifndef SIMPLETIMER_H_
#define SIMPLETIMER_H_

#include <Poco/BasicEvent.h>
#include "Poco/Types.h"
#include "TimerImpl.h"
#include "TimerNotifier.h"
#include "TBS/TBS.h"

namespace TBS {

	class TBS_API SimpleTimer {
		public:
			typedef Poco::SharedPtr <SimpleTimer> Ptr;

			//use default timer notifier
			SimpleTimer();
			//uses given time notifier
			SimpleTimer(TimerNotifier::Ptr tntf);
			virtual ~SimpleTimer();

			void start(long startInterval, long periodicInterval = 0);
			void stop();

			bool isRunning() const;

			typedef Poco::Int64 TimerArg;
			Poco::BasicEvent <TimerArg> Timer;
		private:
			void onTimer(TimerArg & a);
		private:
			bool isRunning_;
			bool isOneShot;
			TimerImpl::Ptr timer;
			TimerNotifier::Ptr notifier;
	};

} /* namespace TBS */
#endif /* SIMPLETIMER_H_ */
