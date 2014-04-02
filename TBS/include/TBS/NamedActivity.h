/*
 * NamedActivity.h
 *
 *  Created on: 9.12.2011
 *      Author: Honza
 */

#ifndef NAMEDACTIVITY_H_
#define NAMEDACTIVITY_H_


#include "Poco/Foundation.h"
#include "Poco/RunnableAdapter.h"
#include "Poco/ThreadPool.h"
#include "Poco/Event.h"
#include "Poco/Mutex.h"
#include "TBS/Log.h"

namespace TBS {

			template <class C>
			class NamedActivity: public Poco::Runnable

			{
				public:
					typedef Poco::RunnableAdapter<C> RunnableAdapterType;
					typedef typename RunnableAdapterType::Callback Callback;

					NamedActivity(std::string name, C* pOwner, Callback method) :
						name(name), _pOwner(pOwner), _runnable(*pOwner, method), _stopped(true), _running(false), _done(false), bgThread(name), joinNeeded(false)
					/// Creates the activity. Call start() to
					/// start it.
					{
						poco_check_ptr (pOwner)
							;
					}

					virtual ~NamedActivity()
					/// Stops and destroys the activity.
					{
						stop();
					}

					void start()
					/// Starts the activity by acquiring a
					/// thread for it from the default thread pool.
					{
						LOG_STREAM_DEBUG << "Start activity " << this->name << LE
						Poco::FastMutex::ScopedLock lock(_mutex);
						if (!_running) {
							_done.reset();
							_stopped = false;
							_running = true;
							try {
								bgThread.start(*this);
								joinNeeded = true;
							} catch (...) {
								_running = false;
								throw;
							}

						}
						LOG_STREAM_DEBUG << "Started activity " << this->name << LE
					}

					void stop() {
						bool joinReallyNeeded = false;
						{
							Poco::FastMutex::ScopedLock lock(_mutex);
							if (_stopped) {
								return;
							}
							_stopped = true;
							joinReallyNeeded = joinNeeded;
							joinNeeded = false;
						}
						try {
							wait(5000);
							LOG_STREAM_DEBUG << "Deleted activity " << this->name << LE
							if (joinReallyNeeded){
								bgThread.join();
							}
						} catch (Poco::Exception & e){
							LOG_STREAM_ERROR << "ERROR NamedActivity "<< this->name << ":" << e.displayText() << LE
							throw;
						}
					}

					void wait() {
						if (_running) {
							_done.wait();
						}
					}

					void wait(long milliseconds) {
						if (_running) {
							try {
								_done.wait(milliseconds);
							} catch (Poco::Exception & e) {
								LOG_STREAM_INFO << "NamedActivity exception: " << e.what() << LE
								throw;
							}
						}
					}

					bool isStopped() const {
						return _stopped;
					}

					bool isRunning() const {
						return _running;
					}

				protected:
					virtual void run() {
						threadDebug();
						LOG_STREAM_DEBUG << "Activity bg started " << this->name  << "[" << Poco::Thread::currentTid() << "] [" << Poco::Thread::current()->id() << "]" /*<< syscall(SYS_gettid)*/ << LE

						try {
							_runnable.run();
						} catch (...) {
							_running = false;
							_done.set();
							throw;
						}
						_running = false;
						_done.set();
						LOG_STREAM_DEBUG << "Activity bg finished " << this->name << "[" << Poco::Thread::currentTid() << "] [" << Poco::Thread::current()->id() << "]" << LE
						//LOG_STREAM_INFO << this->name << " -> LWP " << syscall(SYS_gettid) << " end" << LE
					}

				private:
					NamedActivity();
					NamedActivity(const NamedActivity&);
					NamedActivity& operator =(const NamedActivity&);

					std::string name;
					C* _pOwner;
					RunnableAdapterType _runnable;
					volatile bool _stopped;
					volatile bool _running;
					Poco::Event _done;
					Poco::FastMutex _mutex;

					Poco::Thread bgThread;

					bool joinNeeded;


			};


}

#endif // Foundation_Activity_INCLUDED
