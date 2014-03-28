#ifdef __WIN32__
#ifndef _NO_DBUS
/*
 *
 *  D-Bus++ - C++ bindings for D-Bus
 *
 *  Copyright (C) 2005-2007  Paolo Durante <shackan@gmail.com>
 *
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <dbus-c++/eventloop.h>
#include <dbus-c++/debug.h>

//#include <sys/poll.h>
#include <sys/time.h>

#include <dbus/dbus.h>
#include "Poco/Thread.h"
#include "Poco/Net/Context.h"

#include "winenum.h"

using namespace DBus;
using namespace std;

static double millis(timeval tv) {
	return (tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0);
}

DefaultTimeout::DefaultTimeout(int interval, bool repeat, DefaultMainLoop *ed) :
		_enabled(true), _interval(interval), _repeat(repeat), _expiration(0), _data(0), _disp(ed) {
	timeval now;
	gettimeofday(&now, NULL);

	_expiration = millis(now) + interval;

	_disp->_mutex_t.lock();
	_disp->_timeouts.push_back(this);
	_disp->_mutex_t.unlock();
}

DefaultTimeout::~DefaultTimeout() {
	_disp->_mutex_t.lock();
	_disp->_timeouts.remove(this);
	_disp->_mutex_t.unlock();
}

DefaultWatch::DefaultWatch(int fd, int flags, DefaultMainLoop *ed) :
		_enabled(true), _fd(fd), _flags(flags), _state(0), _data(0), _disp(ed) {
	_disp->_mutex_w.lock();
	std::cout << "default watch add " << std::endl;
	_disp->_watches.push_back(this);
	_disp->_mutex_w.unlock();
}

DefaultWatch::~DefaultWatch() {
	_disp->_mutex_w.lock();
	std::cout << "default watch removed " << std::endl;
	_disp->_watches.remove(this);
	_disp->_mutex_w.unlock();
}

DefaultMutex::DefaultMutex() {

}

DefaultMutex::DefaultMutex(bool recursive) {

}

DefaultMutex::~DefaultMutex() {

}

void DefaultMutex::lock() {
	m.lock();
}

void DefaultMutex::unlock() {
	m.unlock();
}

DefaultMainLoop::DefaultMainLoop() :
		_mutex_w(true) {
}

DefaultMainLoop::~DefaultMainLoop() {
	_mutex_w.lock();

	DefaultWatches::iterator wi = _watches.begin();
	while (wi != _watches.end()) {
		DefaultWatches::iterator wmp = wi;
		++wmp;
		_mutex_w.unlock();
		delete (*wi);
		_mutex_w.lock();
		wi = wmp;
	}
	_mutex_w.unlock();

	_mutex_t.lock();

	DefaultTimeouts::iterator ti = _timeouts.begin();
	while (ti != _timeouts.end()) {
		DefaultTimeouts::iterator tmp = ti;
		++tmp;
		_mutex_t.unlock();
		delete (*ti);
		_mutex_t.lock();
		ti = tmp;
	}
	_mutex_t.unlock();
}
/*
 void DefaultMainLoop::dispatch()
 {
 _mutex_w.lock();

 int nfd = _watches.size();

 if (_fdunlock)
 {
 nfd = nfd + 2;
 }

 pollfd fds[nfd];

 DefaultWatches::iterator wi = _watches.begin();

 for (nfd = 0; wi != _watches.end(); ++wi)
 {
 if ((*wi)->enabled())
 {
 fds[nfd].fd = (*wi)->descriptor();
 fds[nfd].events = (*wi)->flags();
 fds[nfd].revents = 0;

 ++nfd;
 }
 }

 if (_fdunlock)
 {
 fds[nfd].fd = _fdunlock[0];
 fds[nfd].events = POLLIN | POLLOUT | POLLPRI ;
 fds[nfd].revents = 0;

 nfd++;
 fds[nfd].fd = _fdunlock[1];
 fds[nfd].events = POLLIN | POLLOUT | POLLPRI ;
 fds[nfd].revents = 0;
 }

 _mutex_w.unlock();

 int wait_min = 10000;

 DefaultTimeouts::iterator ti;

 _mutex_t.lock();

 for (ti = _timeouts.begin(); ti != _timeouts.end(); ++ti)
 {
 if ((*ti)->enabled() && (*ti)->interval() < wait_min)
 wait_min = (*ti)->interval();
 }

 _mutex_t.unlock();

 poll(fds, nfd, wait_min);

 timeval now;
 gettimeofday(&now, NULL);

 double now_millis = millis(now);

 _mutex_t.lock();

 ti = _timeouts.begin();

 while (ti != _timeouts.end())
 {
 DefaultTimeouts::iterator tmp = ti;
 ++tmp;

 if ((*ti)->enabled() && now_millis >= (*ti)->_expiration)
 {
 (*ti)->expired(*(*ti));

 if ((*ti)->_repeat)
 {
 (*ti)->_expiration = now_millis + (*ti)->_interval;
 }

 }

 ti = tmp;
 }

 _mutex_t.unlock();

 _mutex_w.lock();

 for (int j = 0; j < nfd; ++j)
 {
 DefaultWatches::iterator wi;

 for (wi = _watches.begin(); wi != _watches.end();)
 {
 DefaultWatches::iterator tmp = wi;
 ++tmp;

 if ((*wi)->enabled() && (*wi)->_fd == fds[j].fd)
 {
 if (fds[j].revents)
 {
 (*wi)->_state = fds[j].revents;

 (*wi)->ready(*(*wi));

 fds[j].revents = 0;
 }
 }

 wi = tmp;
 }
 }
 _mutex_w.unlock();
 }
 */

void DefaultMainLoop::dispatch() {

	fd_set watchFdSetRead;
	fd_set watchFdSetWrite;
	fd_set watchFdSetError;

	FD_ZERO(&watchFdSetRead);
	FD_ZERO(&watchFdSetWrite);
	FD_ZERO(&watchFdSetError);

	int maxRead = 0;
	int maxWrite = 0;

	int maxFd = 0;

	{
		Poco::Mutex::ScopedLock l(mutexW);

		for (DefaultWatches::iterator wi = _watches.begin(); wi != _watches.end(); ++wi) {
			if ((*wi)->enabled()){
				if ((*wi)->flags() & DBUS_WATCH_READABLE) {
					FD_SET((*wi)->descriptor(), &watchFdSetRead);
					maxRead++;
				}

				if ((*wi)->flags() & DBUS_WATCH_WRITABLE){
					 FD_SET((*wi)->descriptor(), &watchFdSetWrite);
					 //std::cout << "fdw" << (*wi)->descriptor() << std::endl;
					 maxWrite++;
				}

				FD_SET((*wi)->descriptor(), &watchFdSetError);
			}
		}

		maxFd = maxRead + maxWrite;

		if (maxFd == 0) {
			Poco::Thread::sleep(100);
			return;
		}
	}
	// Now we'll wait in the select call for activity or a timeout

	int wait_min = 10000;

	{
		Poco::Mutex::ScopedLock l(mutexT);

		for (DefaultTimeouts::iterator ti = _timeouts.begin(); ti != _timeouts.end(); ++ti) {
			if ((*ti)->enabled() && (*ti)->interval() < wait_min) {
				wait_min = (*ti)->interval();
			}
		}
	}

	//std::cout << "select read: " << maxRead << " write: " << maxWrite << " wait: " << wait_min << std::endl;

	struct timeval tv;
	tv.tv_sec = wait_min / 1000;
	tv.tv_usec = wait_min % 1000 * 1000;
	int selresult = select(maxFd, &watchFdSetRead, &watchFdSetWrite, &watchFdSetError, &tv);

	if (selresult == 0){
		Poco::Thread::sleep(100);
		std::cout << "select timeout" << std::endl;
		return;
	}

	if (selresult == -1 && errno == EINTR) {
		std::cout << "select interupted" << std::endl;
		//if we were interrupted, continue on
		Poco::Thread::sleep(100);
		return;
	} else if (selresult == -1) {
		std::cout << "select failed " << errno << std::endl;
		Poco::Thread::sleep(100);
		return;
	}


//	Poco::Thread::sleep(100);

	timeval now;
	gettimeofday(&now, NULL);

	double now_millis = millis(now);

	{
		Poco::Mutex::ScopedLock l(mutexT);
		DefaultTimeouts::iterator ti = _timeouts.begin();

		while (ti != _timeouts.end()) {

			DefaultTimeouts::iterator tmp = ti;
			++tmp;

			if ((*ti)->enabled() && now_millis >= (*ti)->_expiration) {
				(*ti)->expired(*(*ti));

				if ((*ti)->_repeat) {
					(*ti)->_expiration = now_millis + (*ti)->_interval;
				}

			}

			ti = tmp;
		}

	}
	{
		Poco::Mutex::ScopedLock l(mutexW);

		for (DefaultWatches::iterator wi = _watches.begin(); wi != _watches.end(); wi++) {
			//DefaultWatches::iterator tmp = wi;
			//++tmp;

			if ((*wi)->enabled()) {
				(*wi)->_state = 0;
				if (FD_ISSET((*wi)->descriptor(), &watchFdSetRead)){
					(*wi)->_state |= POLLIN;
				}
				if (FD_ISSET((*wi)->descriptor(), &watchFdSetWrite)){
					(*wi)->_state |= POLLOUT;
				}
				if (FD_ISSET((*wi)->descriptor(), &watchFdSetError)){
					(*wi)->_state |= POLLERR;
				}
				if ((*wi)->_state){
					//std::cout << "set as ready " << (*wi)->_state << std::endl;
					(*wi)->ready(*(*wi));
				}
			} else {
				//std::cout << "set as not ready" << std::endl;
			}

		}

	}
	Poco::Thread::sleep(100);
}
/*
 _mutex_w.lock();

 int nfd = _watches.size();

 if (_fdunlock)
 {
 nfd = nfd + 2;
 }

 pollfd fds[nfd];

 DefaultWatches::iterator wi = _watches.begin();

 for (nfd = 0; wi != _watches.end(); ++wi)
 {
 if ((*wi)->enabled())
 {
 fds[nfd].fd = (*wi)->descriptor();
 fds[nfd].events = (*wi)->flags();
 fds[nfd].revents = 0;

 ++nfd;
 }
 }

 if (_fdunlock)
 {
 fds[nfd].fd = _fdunlock[0];
 fds[nfd].events = POLLIN | POLLOUT | POLLPRI ;
 fds[nfd].revents = 0;

 nfd++;
 fds[nfd].fd = _fdunlock[1];
 fds[nfd].events = POLLIN | POLLOUT | POLLPRI ;
 fds[nfd].revents = 0;
 }

 _mutex_w.unlock();

 int wait_min = 10000;

 DefaultTimeouts::iterator ti;

 _mutex_t.lock();

 for (ti = _timeouts.begin(); ti != _timeouts.end(); ++ti)
 {
 if ((*ti)->enabled() && (*ti)->interval() < wait_min)
 wait_min = (*ti)->interval();
 }

 _mutex_t.unlock();

 poll(fds, nfd, wait_min);

 timeval now;
 gettimeofday(&now, NULL);

 double now_millis = millis(now);

 _mutex_t.lock();

 ti = _timeouts.begin();

 while (ti != _timeouts.end())
 {
 DefaultTimeouts::iterator tmp = ti;
 ++tmp;

 if ((*ti)->enabled() && now_millis >= (*ti)->_expiration)
 {
 (*ti)->expired(*(*ti));

 if ((*ti)->_repeat)
 {
 (*ti)->_expiration = now_millis + (*ti)->_interval;
 }

 }

 ti = tmp;
 }

 _mutex_t.unlock();

 _mutex_w.lock();

 for (int j = 0; j < nfd; ++j)
 {
 DefaultWatches::iterator wi;

 for (wi = _watches.begin(); wi != _watches.end();)
 {
 DefaultWatches::iterator tmp = wi;
 ++tmp;

 if ((*wi)->enabled() && (*wi)->_fd == fds[j].fd)
 {
 if (fds[j].revents)
 {
 (*wi)->_state = fds[j].revents;

 (*wi)->ready(*(*wi));

 fds[j].revents = 0;
 }
 }

 wi = tmp;
 }
 }
 _mutex_w.unlock();
 */

#endif
#endif
