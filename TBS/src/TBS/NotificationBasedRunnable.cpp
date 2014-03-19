/*
 * NotificationBasedRunnable.cpp
 *
 *  Created on: May 31, 2012
 *      Author: root
 */

#include "TBS/NotificationBasedRunnable.h"
#include "TBS/Log.h"
#include <iostream>
#include "TBS/ThreadStatus.h"


namespace TBS {

	NotificationQueue::NotificationQueue() {
	}

	NotificationQueue::~NotificationQueue() {
		clear();
	}

	void NotificationQueue::enqueueNotification(Poco::Notification::Ptr pNotification) {
		poco_check_ptr(pNotification);

		//LOG_STREAM_TRACE << "NotificationQueue::enqueueNotification before lock" << LOG_STREAM_END;
		Poco::FastMutex::ScopedLock lock(_mutex);
		//LOG_STREAM_TRACE << "NotificationQueue::enqueueNotification after lock" << LOG_STREAM_END;
		if (_waitQueue.empty()) {
			//LOG_STREAM_TRACE << "NotificationQueue::enqueueNotification noone waiting" << LOG_STREAM_END;
			_nfQueue.push_back(pNotification);
		} else {
			//LOG_STREAM_TRACE << "NotificationQueue::enqueueNotification some thread waiting" << LOG_STREAM_END;
			WaitInfo* pWI = _waitQueue.front();
			_waitQueue.pop_front();
			pWI->pNf = pNotification;
			//LOG_STREAM_TRACE << "NotificationQueue::enqueueNotification signal set" << LOG_STREAM_END;
			pWI->nfAvailable.set();
			//LOG_STREAM_TRACE << "NotificationQueue::enqueueNotification signal set done" << LOG_STREAM_END;
		}
	}

	void NotificationQueue::enqueueUrgentNotification(Poco::Notification::Ptr pNotification) {
		poco_check_ptr(pNotification);
		Poco::FastMutex::ScopedLock lock(_mutex);
		if (_waitQueue.empty()) {
			_nfQueue.push_front(pNotification);
		} else {
			WaitInfo* pWI = _waitQueue.front();
			_waitQueue.pop_front();
			pWI->pNf = pNotification;
			pWI->nfAvailable.set();
		}
	}

	Poco::Notification* NotificationQueue::dequeueNotification() {
		//LOG_STREAM_TRACE << "NotificationQueue::dequeueNotification" << LOG_STREAM_END;
		Poco::FastMutex::ScopedLock lock(_mutex);
		return dequeueOne().duplicate();
	}

	Poco::Notification* NotificationQueue::waitDequeueNotification() {
		Poco::Notification::Ptr pNf;
		WaitInfo* pWI = 0;
		{
			//LOG_STREAM_TRACE << "NotificationQueue::enqueueNotification check if something, before lock" << LOG_STREAM_END;
			Poco::FastMutex::ScopedLock lock(_mutex);
			//LOG_STREAM_TRACE << "NotificationQueue::enqueueNotification check if something, locked" << LOG_STREAM_END;
			pNf = dequeueOne();
			if (pNf) {
				//LOG_STREAM_TRACE << "NotificationQueue::enqueueNotification already there" << LOG_STREAM_END;
				return pNf.duplicate();
			}
			pWI = new WaitInfo;
			_waitQueue.push_back(pWI);
		}
		//LOG_STREAM_TRACE << "NotificationQueue::enqueueNotification wait" << LOG_STREAM_END;
		pWI->nfAvailable.wait();
		//LOG_STREAM_TRACE << "NotificationQueue::enqueueNotification signaled" << LOG_STREAM_END;
		pNf = pWI->pNf;
		//LOG_STREAM_DEBUG << "NotificationQueue::enqueueNotification signaled; pNf value:" << (int) pNf.get() << LOG_STREAM_END;
		delete pWI;
		//LOG_STREAM_TRACE << "NotificationQueue::enqueueNotification signaled; after delete pWI" << LOG_STREAM_END;
		Poco::Notification* nf = pNf.duplicate();
		//LOG_STREAM_TRACE << "NotificationQueue::enqueueNotification signaled; after duplicate; ptr: " << nf << "; refCount: " << pNf->referenceCount() << LOG_STREAM_END;
		return nf;
	}

	Poco::Notification* NotificationQueue::waitDequeueNotification(long milliseconds) {
		Poco::Notification::Ptr pNf;
		WaitInfo* pWI = 0;
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			pNf = dequeueOne();
			if (pNf)
			return pNf.duplicate();
			pWI = new WaitInfo;
			_waitQueue.push_back(pWI);
		}
		if (pWI->nfAvailable.tryWait(milliseconds)) {
			pNf = pWI->pNf;
		} else {
			Poco::FastMutex::ScopedLock lock(_mutex);
			pNf = pWI->pNf;
			for (WaitQueue::iterator it = _waitQueue.begin(); it != _waitQueue.end(); ++it) {
				if (*it == pWI) {
					_waitQueue.erase(it);
					break;
				}
			}
		}
		delete pWI;
		return pNf.duplicate();
	}

	void NotificationQueue::wakeUpAll() {
		Poco::FastMutex::ScopedLock lock(_mutex);
		for (WaitQueue::iterator it = _waitQueue.begin(); it != _waitQueue.end(); ++it) {
			(*it)->nfAvailable.set();
		}
		_waitQueue.clear();
	}

	bool NotificationQueue::empty() const {
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _nfQueue.empty();
	}

	int NotificationQueue::size() const {
		Poco::FastMutex::ScopedLock lock(_mutex);
		return static_cast<int>(_nfQueue.size());
	}

	void NotificationQueue::clear() {
		Poco::FastMutex::ScopedLock lock(_mutex);
		_nfQueue.clear();
	}

	bool NotificationQueue::hasIdleThreads() const {
		Poco::FastMutex::ScopedLock lock(_mutex);
		return !_waitQueue.empty();
	}

	Poco::Notification::Ptr NotificationQueue::dequeueOne() {
		LOG_STREAM_TRACE << "NotificationQueue::dequeueOne" << LOG_STREAM_END;
		Poco::Notification::Ptr pNf;
		if (!_nfQueue.empty()) {
			pNf = _nfQueue.front();
			_nfQueue.pop_front();
		}
		return pNf;
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

	NotificationBasedRunnable::NotificationBasedRunnable(std::string threadName)
	: thread(threadName), stopped(false) {

	}

	void NotificationBasedRunnable::stop() {
		LOG_STREAM_TRACE << thread.name() << "(#): NotificationBasedRunnable::stop, before lock" << LOG_STREAM_END;
		Poco::Mutex::ScopedLock l(m);
		LOG_STREAM_TRACE
		<< thread.name() << "(#): NotificationBasedRunnable::stop, after lock" << LOG_STREAM_END;
		this->stopped = true;
		if (this->thread.isRunning()) {
			LOG_STREAM_DEBUG
			<< "Destruct thread " << thread.name() << LOG_STREAM_END;
			this->queue.enqueueUrgentNotification(QuitNotification::Ptr(new QuitNotification()));
			LOG_STREAM_DEBUG
			<< "Destruct thread " << thread.name() << " wait" << LOG_STREAM_END;
			thread.join();
			LOG_STREAM_DEBUG
			<< "Destruct thread " << thread.name() << " done" << LOG_STREAM_END;
		} else {
			LOG_STREAM_DEBUG
			<< "Destruct thread - was not running" << LOG_STREAM_END;
		}
	}
	int NotificationBasedRunnable::queueSize() {
		return this->queue.size();
	}
	NotificationBasedRunnable::~NotificationBasedRunnable() {
		LOG_STREAM_TRACE
		<< thread.name() << "(#): NotificationBasedRunnable::~NotificationBasedRunnable begin" << LOG_STREAM_END;
		this->stop();
		LOG_STREAM_TRACE
		<< thread.name() << "(#): NotificationBasedRunnable::~NotificationBasedRunnable end" << LOG_STREAM_END;
	}

	bool NotificationBasedRunnable::isBgThread() {
		return Poco::Thread::currentTid() == thread.tid();
	}

	void NotificationBasedRunnable::enqueueNotification(Poco::Notification::Ptr notification) {
		LOG_STREAM_TRACE
		<< thread.name() << "(>): NotificationBasedRunnable::enqueueNotification, before lock" << LOG_STREAM_END;
		{
			Poco::Mutex::ScopedLock l(m);
			LOG_STREAM_TRACE
			<< thread.name() << "(>): NotificationBasedRunnable::enqueueNotification, after lock" << LOG_STREAM_END;

			if (stopped) {
				LOG_STREAM_WARNING
				<< thread.name() << "(>): NotificationBasedRunnable::enqueueNotification, stopped" << LOG_STREAM_END;
				return;
			}

			if (!this->thread.isRunning()) {
				LOG_STREAM_TRACE
				<< thread.name() << "(>): NotificationBasedRunnable::enqueueNotification, lazy start" << LOG_STREAM_END;
				this->thread.start(*this);
			}
		}
		LOG_STREAM_DEBUG
		<< thread.name() << "(>): NotificationBasedRunnable::enqueueNotification, before enqueue; queue size: " << this->queue.size()
		<< LOG_STREAM_END;
		this->queue.enqueueNotification(notification);
		LOG_STREAM_TRACE
		<< thread.name() << "(>): NotificationBasedRunnable::enqueueNotification, after enqueue; queue size: " << this->queue.size()
		<< LOG_STREAM_END;
	}

	void NotificationBasedRunnable::run() {

		THREAD_DUMP("run");

		threadDebug();

		LOG_STREAM_DEBUG
		<< thread.name() << ": bg thread started" << LOG_STREAM_END;
		try {
			Poco::Notification::Ptr pNf(queue.waitDequeueNotification());

			while (pNf) {

				Poco::Timestamp t;
				LOG_NAMED_STREAM_INFO(LOG_THREAD) << thread.name() << "(>): Accept notification '" << pNf->name() << "' queue size is " << queue.size() << LOG_STREAM_END

				//quit notification
				QuitNotification::Ptr pQuitNf = pNf.cast<QuitNotification>();
				if (pQuitNf) {
					LOG_NAMED_STREAM_INFO(LOG_THREAD)
					<< thread.name() << "(>): Quit" << LOG_STREAM_END;
					break;
				}

				this->processNotification(pNf);

				THREAD_DUMP("ntf: " + pNf->name());

				LOG_NAMED_STREAM_INFO(LOG_THREAD)
				<< thread.name() << "(>): Process notification '" << pNf->name() << "' finished in " << t.elapsed() / 1000 << "ms" << LOG_STREAM_END
				pNf = queue.waitDequeueNotification();

				//LOG_NAMED_STREAM_INFO(LOG_THREAD)
				//		<< thread.name() << "(>): After waitDequeueNotification, ptr: " << (int) pNf.get() << LOG_STREAM_END
				if (!pNf.isNull()) {
					//LOG_NAMED_STREAM_INFO(LOG_THREAD)
					//		<< thread.name() << "(>): After waitDequeueNotification, name: '" << pNf->name() << LOG_STREAM_END
				}
			}
			THREAD_DUMP("finished ok");
			LOG_NAMED_STREAM_INFO(LOG_THREAD)
			<< thread.name() << "(>): Out from while cycle; ptr: " << (long int) pNf.get() << LOG_STREAM_END
		} catch (Poco::Exception & e) {
			THREAD_DUMP("finished poco exc");
			LOG_NAMED_STREAM_FATAL(LOG_THREAD)
			<< thread.name() << ": Worker thread finished with exception " << e.displayText() << LOG_STREAM_END;
		} catch (std::runtime_error & e) {
			THREAD_DUMP("finished re exc");
			LOG_NAMED_STREAM_FATAL(LOG_THREAD)
			<< thread.name() << ": Worker thread finished with std runtime exception " << e.what() << LOG_STREAM_END;
		} catch (std::exception & e) {
			THREAD_DUMP("finished e exc");
			LOG_NAMED_STREAM_FATAL(LOG_THREAD)
			<< thread.name() << ": Worker thread finished with std exception " << e.what() << LOG_STREAM_END;
		} catch (...) {
			THREAD_DUMP("finished ... exc");
			LOG_NAMED_STREAM_FATAL(LOG_THREAD)
			<< thread.name() << ": Worker thread finished with unknown exception" << LOG_STREAM_END;
		}
		LOG_STREAM_DEBUG
		<< thread.name() << ": bg thread finished" << LOG_STREAM_END;
	}

} /* namespace TBS */
