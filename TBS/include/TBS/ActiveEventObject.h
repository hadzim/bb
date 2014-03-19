/*
 * ActiveObject.h
 *
 *  Created on: May 22, 2011
 *      Author: dron
 */

#ifndef ACTIVEEVENTOBJECT_H_
#define ACTIVEEVENTOBJECT_H_

#include "TBS/TBS.h"

#include <Poco/BasicEvent.h>
#include <Poco/Delegate.h>

namespace TBS {

	enum Activation {
		Passivating = 0, Activating = 1
	};

	class TBS_API ActiveEventObject {

		public:
			ActiveEventObject();
			~ActiveEventObject();

			typedef bool Args;
			typedef Poco::BasicEvent<Args> Event;
			typedef Activation ActivationArgs;
			typedef Poco::BasicEvent<ActivationArgs> ActivationEvent;

		public:
			bool isActive();

			void activate();
			void passivate();

		public:
			//interface
			//each event separately
			Event BeforeActivate;
			Event AfterActivate;
			Event BeforePassivate;
			Event AfterPassivate;

			///joined AfterActivate and BeeforePassivate
			ActivationEvent InnerActivation;
			///joined BeforeActivate and AfterPassivate
			ActivationEvent OuterActivation;
		private:

			Poco::Mutex mutex;
			bool active;

	};

}
#endif /* ACTIVEOBJECT_H_ */
