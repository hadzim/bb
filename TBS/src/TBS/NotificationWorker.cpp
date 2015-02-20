/*
 * NotificationWorker.cpp
 *
 *  Created on: 30.11.2011
 *      Author: Honza
 */

#include "TBS/NotificationWorker.h"

#include <iostream>
#include "TBS/Log.h"
#include "Poco/AbstractObserver.h"
#include "TBS/CommandNotification.h"
#include "Poco/Exception.h"

namespace TBS {

	class CommandObserver: public Poco::AbstractObserver {
		public:
			virtual ~CommandObserver() {
			}
			virtual void notify(Poco::Notification* pNf) const {
				CommandNotification* pCastNf = dynamic_cast<CommandNotification*>(pNf);
				if (pCastNf) {
					LTRACE("CMD")<< "CMD " << pCastNf->name() << " observed" << LE
						CommandNotification::Ptr ptr(pCastNf, true);
						pCastNf->run();
						LTRACE("CMD") << "CMD " << pCastNf->name() << " finished" << LE
					}
				}
				virtual bool equals(const Poco::AbstractObserver& observer) const {
					const CommandObserver* pObs = dynamic_cast<const CommandObserver*>(&observer);
					return pObs;
				}
				virtual bool accepts(Poco::Notification* pNf) const {
					return dynamic_cast<CommandNotification*>(pNf) != 0;
				}
				virtual Poco::AbstractObserver* clone() const {
					return new CommandObserver();
				}
				virtual void disable() {
				}

			};

	NotificationWorker::NotificationWorker(std::string name_) :
			NotificationBasedRunnable(name_), name(name_) {
		LTRACE("CMD")<< "construct worker " << name << LE
			addCommandObserver(*this);
		}

	NotificationWorker::~NotificationWorker() {

		LTRACE("CMD")<< "delete worker " << name << " queue size: " << this->queueSize() << LE

			this->stop();

			removeCommandObserver(*this);
			//this->removeObserver(CommandObserver());

			LTRACE("CMD") << "delete worker ok queue " << this->queueSize() << " observers: " << this->center.countObservers() << LE

		}

	bool NotificationWorker::isBgThread() {
		return NotificationBasedRunnable::isBgThread();
	}

	void NotificationWorker::processNotification(Poco::Notification::Ptr notification) {
		try {

			LTRACE(LOG_THREAD)<< this->name << "(>): Process notification '" << notification->name() << "'" << LE

			center.postNotification(notification);

			LTRACE(LOG_THREAD) << this->name << "(>): Process notification '" << notification->name() << "' finished size is " << this->queueSize() << LE

		} catch (Poco::Exception & e) {
			LWARNING(LOG_THREAD) << "NotificationWorker::run " << " event exception " << e.displayText() << LE
		}
	}

	void NotificationWorker::addObserver(const Poco::AbstractObserver& observer) {
		this->center.addObserver(observer);
	}
	void NotificationWorker::removeObserver(const Poco::AbstractObserver& observer) {
		this->center.removeObserver(observer);
	}

	void NotificationWorker::postNotification(Poco::Notification::Ptr pNotification) {
		LTRACE(LOG_THREAD)<< this->name << "(<): Post notification '" << pNotification->name() << "' (queue size is " << this->queueSize() << ")" << LE
		this->enqueueNotification(pNotification);

	}

	NotificationObserver::NotificationObserver() :
			observer(NULL) {

	}
	NotificationObserver::~NotificationObserver() {
		this->reset();
	}

	void NotificationObserver::reset() {
		if (observer) {
			this->nw->removeObserver(*this->observer);
			delete observer;
			this->observer = NULL;
		}
	}

	void NotificationObserver::set(Poco::AbstractObserver * o) {
		if (observer) {
			throw Poco::LogicException("Observer multi inicialization");
		}
		this->observer = o;
		this->nw->addObserver(*this->observer);
	}

	NotificationObservers::NotificationObservers(INotificationWorker::Ptr nw) :
			nw(nw) {

	}
	NotificationObservers::~NotificationObservers() {
		this->clear();
	}

	void NotificationObservers::addInternally(Poco::AbstractObserver * o) {
		this->nw->addObserver(*o);
		this->observers.push_back(o);
	}

	void NotificationObservers::clear() {
		for (Observers::iterator oi = observers.begin(); oi != observers.end(); oi++) {
			this->nw->removeObserver(*(*oi));
			delete *oi;
		}
		observers.clear();
	}

	void addCommandObserver(INotificationWorker & nw) {
		nw.addObserver(CommandObserver());
	}
	void removeCommandObserver(INotificationWorker & nw) {
		nw.removeObserver(CommandObserver());
	}

}

