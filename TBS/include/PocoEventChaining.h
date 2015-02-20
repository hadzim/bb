#ifndef INCLUDED_POCO_EVENT_CHAINING
#define INCLUDED_POCO_EVENT_CHAINING

/*
	Some templated overloaded operators for chaining Poco basic events.
*/

#include "Poco/BasicEvent.h"
#include "Poco/Delegate.h"
#include "Poco/AbstractDelegate.h"
#include <memory>
#include <iostream>


class IEventForwarder {
	public:
		typedef std::unique_ptr <IEventForwarder> AutoPtr;
		virtual ~IEventForwarder() {}
		virtual void enable(void) = 0;
		virtual void disable(void) = 0;
		virtual bool isEnabled(void) = 0;
};

template <class T>
class EventForwarder: public IEventForwarder {
	public:
		typedef std::unique_ptr <EventForwarder<T> > AutoPtr;
		EventForwarder(Poco::BasicEvent<T>& from, Poco::BasicEvent<T>& to) : from(from), to(to), enabled(false) {
			from += Poco::delegate(this, &EventForwarder<T>::fwd);
			enabled = true;
		}
		~EventForwarder(){
			from -= Poco::delegate(this, &EventForwarder<T>::fwd);
			enabled = false;
		}
		void enable(void) { 
			if (!enabled) {
				from += Poco::delegate(this, &EventForwarder<T>::fwd);
				enabled = true;
			}
		}

		void disable(void) {
			if (enabled) {
				from -= Poco::delegate(this, &EventForwarder<T>::fwd);
				enabled = false;
			}
		}

		bool isEnabled(void) { return enabled; }

	private:
		void fwd(T & val){ to.notify(this, val); }

		Poco::BasicEvent<T>& from;
		Poco::BasicEvent<T>& to;
		bool enabled;

};

template <class T>
IEventForwarder::AutoPtr forwardFromTo(Poco::BasicEvent<T>& from, Poco::BasicEvent<T>& to)
{
	IEventForwarder::AutoPtr forwarder;
	forwarder.reset(new EventForwarder<T>(from, to));
	return forwarder;
}


#endif // included

