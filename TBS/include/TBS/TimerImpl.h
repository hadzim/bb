/*
 * TimerImpl.h
 *
 *  Created on: 25.3.2014
 *      Author: JV
 */

#ifndef SHAREDTIMERIMPL_H_
#define SHAREDTIMERIMPL_H_

#include <memory>
#include <Poco/SharedPtr.h>
#include <Poco/BasicEvent.h>
#include "Poco/Types.h"
#include "TBS/TBS.h"

namespace TBS {

	class  TBS_API TimerImpl {
		public:
			typedef Poco::SharedPtr <TimerImpl> Ptr;

			TimerImpl();


			typedef Poco::Int64 TimerArg;
			Poco::BasicEvent <TimerArg> Timer;

	};

} /* namespace TBS */
#endif /* SHAREDTIMERIMPL_H_ */
