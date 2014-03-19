/*
 * PrioNotificationBasedRunnable.cpp
 *
 *  Created on: Sep 17, 2012
 *      Author: root
 */

#include "TBS/PrioNotificationBasedRunnable.h"
#include "TBS/Log.h"
#include <iostream>
#include <limits.h>

namespace TBS {


		PrioNotificationBasedRunnable::PrioNotificationBasedRunnable(std::string threadName) :
			thread(threadName), stopped(false) {
		}

		PrioNotificationBasedRunnable::~PrioNotificationBasedRunnable() {
			LOG_STREAM_TRACE << thread.name() << "(#): PrioNotificationBasedRunnable::~PrioNotificationBasedRunnable begin" << LOG_STREAM_END;
			this->stop();
			LOG_STREAM_TRACE << thread.name() << "(#): PrioNotificationBasedRunnable::~PrioNotificationBasedRunnable end" << LOG_STREAM_END;
		}

		void PrioNotificationBasedRunnable::enqueueNotification(Poco::Notification::Ptr notification, int priority) {
			LOG_STREAM_TRACE << thread.name() << "(>): PrioNotificationBasedRunnable::enqueueNotification prio, before lock" << LOG_STREAM_END;
			{
				Poco::Mutex::ScopedLock l(m);
				LOG_STREAM_TRACE << thread.name() << "(>): PrioNotificationBasedRunnable::enqueueNotification prio, after lock" << LOG_STREAM_END;

				if (stopped) {
					LOG_STREAM_WARNING << thread.name() << "(>): PrioNotificationBasedRunnable::enqueueNotification prio, stopped" << LOG_STREAM_END;
					return;
				}

				if (!this->thread.isRunning()) {
					LOG_STREAM_TRACE << thread.name() << "(>): PrioNotificationBasedRunnable::enqueueNotification prio, lazy start" << LOG_STREAM_END;
					this->thread.start(*this);
				}
			}
			LOG_STREAM_DEBUG << thread.name() << "(>): PrioNotificationBasedRunnable::enqueueNotification prio, before enqueue; queue size: "
					<< this->queue.size() << LOG_STREAM_END;
			this->queue.enqueueNotification(notification, priority);
			LOG_STREAM_TRACE << thread.name() << "(>): PrioNotificationBasedRunnable::enqueueNotification prio, after enqueue; queue size: "
					<< this->queue.size() << LOG_STREAM_END;
		}

		class QuitNotification: public Poco::Notification {

			public:
				typedef Poco::AutoPtr<QuitNotification> Ptr;

				QuitNotification() {

				}
				virtual ~QuitNotification() {

				}

				virtual std::string name() const {
					return "QuitNotification";
				}

		};

		void PrioNotificationBasedRunnable::stop() {
			LOG_STREAM_TRACE << thread.name() << "(#): PrioNotificationBasedRunnable::stop, before lock" << LOG_STREAM_END;
			Poco::Mutex::ScopedLock l(m);
			LOG_STREAM_TRACE << thread.name() << "(#): PrioNotificationBasedRunnable::stop, after lock" << LOG_STREAM_END;
			this->stopped = true;
			if (this->thread.isRunning()) {
				LOG_STREAM_DEBUG << "Destruct thread " << thread.name() << LOG_STREAM_END;
				this->queue.enqueueNotification(QuitNotification::Ptr(new QuitNotification()), INT_MIN);
				LOG_STREAM_DEBUG << "Destruct thread " << thread.name() << " wait" << LOG_STREAM_END;
				thread.join();
				LOG_STREAM_DEBUG << "Destruct thread " << thread.name() << " done" << LOG_STREAM_END;
			} else {
				LOG_STREAM_DEBUG << "Destruct thread - was not running" << LOG_STREAM_END;
			}
		}

		int PrioNotificationBasedRunnable::queueSize() {
			return this->queue.size();
		}

		bool PrioNotificationBasedRunnable::isBgThread() {
			return Poco::Thread::currentTid() == thread.tid();
		}

		void PrioNotificationBasedRunnable::run() {
			threadDebug();

			LOG_STREAM_DEBUG << thread.name() << ": bg thread started" << LOG_STREAM_END;
			try {
				Poco::Notification::Ptr pNf(queue.waitDequeueNotification());

				while (pNf) {

					Poco::Timestamp t;
					LOG_NAMED_STREAM_INFO(LOG_THREAD) << thread.name() << "(>): Accept notification '" << pNf->name() << "'" << LOG_STREAM_END;

					//quit notification
					QuitNotification::Ptr pQuitNf = pNf.cast<QuitNotification>();
					if (pQuitNf) {
						LOG_NAMED_STREAM_INFO(LOG_THREAD) << thread.name() << "(>): Quit" << LOG_STREAM_END;
						break;
					}

					this->processNotification(pNf);

				LOG_NAMED_STREAM_INFO(LOG_THREAD) << thread.name() << "(>): Process notification '" << pNf->name() << "' finished in " << t.elapsed() / 1000 << "ms" << LOG_STREAM_END;
				pNf = queue.waitDequeueNotification();
				LOG_NAMED_STREAM_INFO(LOG_THREAD) << thread.name() << "(>): After waitDequeueNotification, ptr: " << (long int) pNf.get() << LOG_STREAM_END;
				if (!pNf.isNull()) {
					LOG_NAMED_STREAM_INFO(LOG_THREAD) << thread.name() << "(>): After waitDequeueNotification, name: '" << pNf->name() << LOG_STREAM_END;
				}
			}
			LOG_NAMED_STREAM_INFO(LOG_THREAD) << thread.name() << "(>): Out from while cycle; ptr: " << (long int) pNf.get() << LOG_STREAM_END;
		} catch (Poco::Exception & e) {
			LOG_NAMED_STREAM_FATAL(LOG_THREAD) << thread.name() << ": Worker thread finished with exception " << e.displayText() << LOG_STREAM_END;
		} catch (std::runtime_error & e) {
			LOG_NAMED_STREAM_FATAL(LOG_THREAD) << thread.name() << ": Worker thread finished with std runtime exception " << e.what() << LOG_STREAM_END;
		} catch (std::exception & e) {
			LOG_NAMED_STREAM_FATAL(LOG_THREAD) << thread.name() << ": Worker thread finished with std exception " << e.what() << LOG_STREAM_END;
		} catch (...) {
			LOG_NAMED_STREAM_FATAL(LOG_THREAD) << thread.name() << ": Worker thread finished with unknown exception" << LOG_STREAM_END;
		}
		LOG_STREAM_DEBUG << thread.name() << ": bg thread finished" << LOG_STREAM_END;
	}

}
