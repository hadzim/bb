/*
 * ConnectionManager.h
 *
 *  Created on: Jan 21, 2014
 *      Author: dron
 */

#ifndef CONNECTIONMANAGER_H_
#define CONNECTIONMANAGER_H_
#include "Poco/Mutex.h"
#include <deque>
#include <map>
#include <unistd.h>

namespace DBus {

	class ConnectionManager {
		public:
			ConnectionManager();
			virtual ~ConnectionManager();

			bool isDeleted(intptr_t ptr);
			void setAsDeleted(intptr_t ptr);

			static ConnectionManager & instance();
		private:
			static Poco::Mutex globalm;
			Poco::Mutex m;
			typedef std::deque<intptr_t> DeletedQueue;
			typedef std::map <intptr_t, bool> Deleted;
			DeletedQueue deletedQueue;
			Deleted deleted;
	};

} /* namespace DBus */

#endif /* CONNECTIONMANAGER_H_ */
