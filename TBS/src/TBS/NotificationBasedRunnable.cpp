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
/*
	NotificationQueue::NotificationQueue() {
	}

	NotificationQueue::~NotificationQueue() {
		clear();
	}

	void NotificationQueue::enqueueNotification(Poco::Notification::Ptr pNotification) {
		poco_check_ptr(pNotification);

		//LOG_STREAM_TRACE << "NotificationQueue::enqueueNotification before lock" << LE;
		Poco::FastMutex::ScopedLock lock(_mutex);
		//LOG_STREAM_TRACE << "NotificationQueue::enqueueNotification after lock" << LE;
		if (_waitQueue.empty()) {
			//LOG_STREAM_TRACE << "NotificationQueue::enqueueNotification noone waiting" << LE;
			_nfQueue.push_back(pNotification);
		} else {
			//LOG_STREAM_TRACE << "NotificationQueue::enqueueNotification some thread waiting" << LE;
			WaitInfo* pWI = _waitQueue.front();
			_waitQueue.pop_front();
			pWI->pNf = pNotification;
			//LOG_STREAM_TRACE << "NotificationQueue::enqueueNotification signal set" << LE;
			pWI->nfAvailable.set();
			//LOG_STREAM_TRACE << "NotificationQueue::enqueueNotification signal set done" << LE;
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
		//LOG_STREAM_TRACE << "NotificationQueue::dequeueNotification" << LE;
		Poco::FastMutex::ScopedLock lock(_mutex);
		return dequeueOne().duplicate();
	}

	Poco::Notification* NotificationQueue::waitDequeueNotification() {
		Poco::Notification::Ptr pNf;
		WaitInfo* pWI = 0;
		{
			//LOG_STREAM_TRACE << "NotificationQueue::enqueueNotification check if something, before lock" << LE;
			Poco::FastMutex::ScopedLock lock(_mutex);
			//LOG_STREAM_TRACE << "NotificationQueue::enqueueNotification check if something, locked" << LE;
			pNf = dequeueOne();
			if (pNf) {
				//LOG_STREAM_TRACE << "NotificationQueue::enqueueNotification already there" << LE;
				return pNf.duplicate();
			}
			pWI = new WaitInfo;
			_waitQueue.push_back(pWI);
		}
		//LOG_STREAM_TRACE << "NotificationQueue::enqueueNotification wait" << LE;
		pWI->nfAvailable.wait();
		//LOG_STREAM_TRACE << "NotificationQueue::enqueueNotification signaled" << LE;
		pNf = pWI->pNf;
		//LOG_STREAM_DEBUG << "NotificationQueue::enqueueNotification signaled; pNf value:" << (int) pNf.get() << LE;
		delete pWI;
		//LOG_STREAM_TRACE << "NotificationQueue::enqueueNotification signaled; after delete pWI" << LE;
		Poco::Notification* nf = pNf.duplicate();
		//LOG_STREAM_TRACE << "NotificationQueue::enqueueNotification signaled; after duplicate; ptr: " << nf << "; refCount: " << pNf->referenceCount() << LE;
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
		LOG_STREAM_TRACE << "NotificationQueue::dequeueOne" << LE;
		Poco::Notification::Ptr pNf;
		if (!_nfQueue.empty()) {
			pNf = _nfQueue.front();
			_nfQueue.pop_front();
		}
		return pNf;
	}
*/
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
		LOG_STREAM_TRACE << thread.name() << "(#): NotificationBasedRunnable::stop, before lock" << LE;
		Poco::Mutex::ScopedLock l(m);
		LOG_STREAM_TRACE
		<< thread.name() << "(#): NotificationBasedRunnable::stop, after lock" << LE;
		this->stopped = true;
		if (this->thread.isRunning()) {
			LDEBUG(LOG_THREAD)
			<< "Destruct thread " << thread.name() << LE;
			this->queue.enqueueUrgentNotification(QuitNotification::Ptr(new QuitNotification()));
			LDEBUG(LOG_THREAD)
			<< "Destruct thread " << thread.name() << " wait" << LE;
			thread.join();
			LDEBUG(LOG_THREAD)
			<< "Destruct thread " << thread.name() << " done" << LE;
		} else {
			LDEBUG(LOG_THREAD)
			<< "Destruct thread - was not running" << LE;
		}
	}
	int NotificationBasedRunnable::queueSize() {
		return this->queue.size();
	}
	NotificationBasedRunnable::~NotificationBasedRunnable() {
		LTRACE(LOG_THREAD)
		<< thread.name() << "(#): NotificationBasedRunnable::~NotificationBasedRunnable begin" << LE;
		this->stop();
		LTRACE(LOG_THREAD)
		<< thread.name() << "(#): NotificationBasedRunnable::~NotificationBasedRunnable end" << LE;
	}

	bool NotificationBasedRunnable::isBgThread() {
		return Poco::Thread::currentTid() == thread.tid();
	}

	void NotificationBasedRunnable::enqueueNotification(Poco::Notification::Ptr notification) {
		LTRACE(LOG_THREAD)
		<< thread.name() << "(>): NotificationBasedRunnable::enqueueNotification, before lock" << LE;
		{
			Poco::Mutex::ScopedLock l(m);
			LTRACE(LOG_THREAD)
			<< thread.name() << "(>): NotificationBasedRunnable::enqueueNotification, after lock" << LE;

			if (stopped) {
				LWARNING(LOG_THREAD)
				<< thread.name() << "(>): NotificationBasedRunnable::enqueueNotification, stopped" << LE;
				return;
			}

			if (!this->thread.isRunning()) {
				LTRACE(LOG_THREAD)
				<< thread.name() << "(>): NotificationBasedRunnable::enqueueNotification, lazy start" << LE;
				this->thread.start(*this);
			}
		}
		LINFO(LOG_THREAD) << thread.name() << "(>): Enqueue Notification " << notification->name() << ", queue size before: " << this->queue.size() << LE;
		this->queue.enqueueNotification(notification);
		LTRACE(LOG_THREAD)
		<< thread.name() << "(>): NotificationBasedRunnable::enqueueNotification, after enqueue; queue size: " << this->queue.size()
		<< LE;
	}

	void NotificationBasedRunnable::run() {

		THREAD_DUMP("run");

		threadDebug();

		LINFO(LOG_THREAD)
		<< thread.name() << ": bg thread started" << LE;
		try {
			Poco::Notification::Ptr pNf(queue.waitDequeueNotification());

			while (pNf) {

				Poco::Timestamp t;
				LINFO(LOG_THREAD)  << thread.name() << "(>): Accept: '" << pNf->name() << "' queue size is " << queue.size() << LE

				//quit notification
				QuitNotification::Ptr pQuitNf = pNf.cast<QuitNotification>();
				if (pQuitNf) {
					LINFO(LOG_THREAD) << thread.name() << "(>): Quit" << LE;
					break;
				}

				this->processNotification(pNf);

				THREAD_DUMP("ntf: " + pNf->name());

				LINFO(LOG_THREAD) << thread.name() << "(>): Process: '" << pNf->name() << "' finished in " << t.elapsed() / 1000 << "ms" << LE
				pNf = queue.waitDequeueNotification();

			}
			THREAD_DUMP("finished ok");
			LDEBUG(LOG_THREAD)
			<< thread.name() << "(>): Out from while cycle; ptr: " << (long int) pNf.get() << LE;
		} catch (Poco::Exception & e) {
			THREAD_DUMP("finished poco exc");
			LFATAL(LOG_THREAD)
			<< thread.name() << ": Worker thread finished with exception " << e.displayText() << LE;
		} catch (std::exception & e) {
			THREAD_DUMP("finished e exc");
			LFATAL(LOG_THREAD)
			<< thread.name() << ": Worker thread finished with std exception " << e.what() << LE;
		} catch (...) {
			THREAD_DUMP("finished ... exc");
			LFATAL(LOG_THREAD)
			<< thread.name() << ": Worker thread finished with unknown exception" << LE;
		}
		LDEBUG(LOG_THREAD)
		<< thread.name() << ": bg thread finished" << LE;
	}

} /* namespace TBS */
