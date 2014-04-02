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
			LOG_STREAM_TRACE << thread.name() << "(#): PrioNotificationBasedRunnable::~PrioNotificationBasedRunnable begin" << LE;
			this->stop();
			LOG_STREAM_TRACE << thread.name() << "(#): PrioNotificationBasedRunnable::~PrioNotificationBasedRunnable end" << LE;
		}

		void PrioNotificationBasedRunnable::enqueueNotification(Poco::Notification::Ptr notification, int priority) {
			LOG_STREAM_TRACE << thread.name() << "(>): PrioNotificationBasedRunnable::enqueueNotification prio, before lock" << LE;
			{
				Poco::Mutex::ScopedLock l(m);
				LOG_STREAM_TRACE << thread.name() << "(>): PrioNotificationBasedRunnable::enqueueNotification prio, after lock" << LE;

				if (stopped) {
					LOG_STREAM_WARNING << thread.name() << "(>): PrioNotificationBasedRunnable::enqueueNotification prio, stopped" << LE;
					return;
				}

				if (!this->thread.isRunning()) {
					LOG_STREAM_TRACE << thread.name() << "(>): PrioNotificationBasedRunnable::enqueueNotification prio, lazy start" << LE;
					this->thread.start(*this);
				}
			}
			LOG_STREAM_DEBUG << thread.name() << "(>): PrioNotificationBasedRunnable::enqueueNotification prio, before enqueue; queue size: "
					<< this->queue.size() << LE;
			this->queue.enqueueNotification(notification, priority);
			LOG_STREAM_TRACE << thread.name() << "(>): PrioNotificationBasedRunnable::enqueueNotification prio, after enqueue; queue size: "
					<< this->queue.size() << LE;
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
			LOG_STREAM_TRACE << thread.name() << "(#): PrioNotificationBasedRunnable::stop, before lock" << LE;
			Poco::Mutex::ScopedLock l(m);
			LOG_STREAM_TRACE << thread.name() << "(#): PrioNotificationBasedRunnable::stop, after lock" << LE;
			this->stopped = true;
			if (this->thread.isRunning()) {
				LOG_STREAM_DEBUG << "Destruct thread " << thread.name() << LE;
				this->queue.enqueueNotification(QuitNotification::Ptr(new QuitNotification()), INT_MIN);
				LOG_STREAM_DEBUG << "Destruct thread " << thread.name() << " wait" << LE;
				thread.join();
				LOG_STREAM_DEBUG << "Destruct thread " << thread.name() << " done" << LE;
			} else {
				LOG_STREAM_DEBUG << "Destruct thread - was not running" << LE;
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

			LOG_STREAM_DEBUG << thread.name() << ": bg thread started" << LE;
			try {
				Poco::Notification::Ptr pNf(queue.waitDequeueNotification());

				while (pNf) {

					Poco::Timestamp t;
					LOG_NAMED_STREAM_INFO(LOG_THREAD) << thread.name() << "(>): Accept notification '" << pNf->name() << "'" << LE;

					//quit notification
					QuitNotification::Ptr pQuitNf = pNf.cast<QuitNotification>();
					if (pQuitNf) {
						LOG_NAMED_STREAM_INFO(LOG_THREAD) << thread.name() << "(>): Quit" << LE;
						break;
					}

					this->processNotification(pNf);

				LOG_NAMED_STREAM_INFO(LOG_THREAD) << thread.name() << "(>): Process notification '" << pNf->name() << "' finished in " << t.elapsed() / 1000 << "ms" << LE;
				pNf = queue.waitDequeueNotification();
				LOG_NAMED_STREAM_INFO(LOG_THREAD) << thread.name() << "(>): After waitDequeueNotification, ptr: " << (long int) pNf.get() << LE;
				if (!pNf.isNull()) {
					LOG_NAMED_STREAM_INFO(LOG_THREAD) << thread.name() << "(>): After waitDequeueNotification, name: '" << pNf->name() << LE;
				}
			}
			LOG_NAMED_STREAM_INFO(LOG_THREAD) << thread.name() << "(>): Out from while cycle; ptr: " << (long int) pNf.get() << LE;
		} catch (Poco::Exception & e) {
			LOG_NAMED_STREAM_FATAL(LOG_THREAD) << thread.name() << ": Worker thread finished with exception " << e.displayText() << LE;
		} catch (std::runtime_error & e) {
			LOG_NAMED_STREAM_FATAL(LOG_THREAD) << thread.name() << ": Worker thread finished with std runtime exception " << e.what() << LE;
		} catch (std::exception & e) {
			LOG_NAMED_STREAM_FATAL(LOG_THREAD) << thread.name() << ": Worker thread finished with std exception " << e.what() << LE;
		} catch (...) {
			LOG_NAMED_STREAM_FATAL(LOG_THREAD) << thread.name() << ": Worker thread finished with unknown exception" << LE;
		}
		LOG_STREAM_DEBUG << thread.name() << ": bg thread finished" << LE;
	}

}
