/*
 * ConnectionManager.cpp
 *
 *  Created on: Jan 21, 2014
 *      Author: dron
 */

#include "ConnectionManager.h"

namespace DBus {

	Poco::Mutex ConnectionManager::globalm;

	ConnectionManager::ConnectionManager() {

	}

	ConnectionManager::~ConnectionManager() {

	}

	bool ConnectionManager::isDeleted(int ptr) {
		Poco::Mutex::ScopedLock l(m);
		if (deleted.find(ptr) != deleted.end()){
			return deleted[ptr];
		}
		return false;
	}
	void ConnectionManager::setAsDeleted(int ptr) {
		Poco::Mutex::ScopedLock l(m);
		if (deleted.find(ptr) != deleted.end()){
			deleted[ptr] = true;
		} else {
			deletedQueue.push_back(ptr);
			deleted[ptr] = true;
		}

		while (deleted.size() > 100 && !deletedQueue.empty()){
			int act = deletedQueue.front();
			deletedQueue.pop_front();
			deleted.erase(act);
		}
	}

	ConnectionManager & ConnectionManager::instance() {
		Poco::Mutex::ScopedLock l(globalm);
		static ConnectionManager d;
		return d;
	}

} /* namespace DBus */
