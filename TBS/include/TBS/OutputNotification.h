/*
 * NotificationWorker.h
 *
 *  Created on: 30.11.2011
 *      Author: Honza
 */

#ifndef TBSTSUNOTIFICATIONWORKER_H_
#define TBSTSUNOTIFICATIONWORKER_H_
#include "Poco/Runnable.h"
#include <Poco/Observer.h>
#include <Poco/Notification.h>
#include <Poco/Thread.h>
#include <Poco/NotificationCenter.h>
#include <Poco/NotificationQueue.h>
#include <Poco/SharedPtr.h>
#include <Poco/BasicEvent.h>
#include "TBS/Command.h"
#include "TBS/Log.h"

namespace TBS {
	template<class Argument>
	class OutputEventCommand: public ICommand {
		public:
			OutputEventCommand(std::string name, void * object, Poco::BasicEvent<Argument> & event, Argument & value) :
					ICommand("OutputEvent::" + name), object(object), event(event), value(value) {
			}
			virtual void run() {
				event.notify(object, value);
			}
		private:
			void * object;
			Poco::BasicEvent<Argument> & event;
			Argument value;

	};

	template<class Argument>
	class OutputEventCommandCheckable: public ICommand {
		public:
			typedef Poco::SharedPtr<OutputEventCommandCheckable<Argument> > Ptr;
			OutputEventCommandCheckable(std::string name, void * object, Poco::BasicEvent<Argument> & event, Argument & value) :
					ICommand("OutputEventCheckable::" + name), object(object), event(event), value(value) {
				check = new Poco::Event();
			}
			virtual void run() {
				try {
					event.notify(object, value);
					check->set();
				} catch (Poco::Exception & e) {
					LERROR("Event")<<"OutputEventCommandCheckable ERROR: " << this->getName() << " finished exc "<< e.displayText() << LE;
					check->set();
					throw;
				} catch (...) {
					LERROR("Event") << "OutputEventCommandCheckable ERROR: " << LE;
					check->set();
					throw;
				}
			}

			Poco::SharedPtr <Poco::Event> getCheck() {
				return this->check;
			}

			private:
			void * object;
			Poco::BasicEvent<Argument> & event;
			Argument value;
			Poco::SharedPtr <Poco::Event> check;
		};

	}
#endif /* NOTIFICATIONWORKER_H_ */
