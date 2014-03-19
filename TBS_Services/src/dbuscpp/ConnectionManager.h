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

namespace DBus {

	class ConnectionManager {
		public:
			ConnectionManager();
			virtual ~ConnectionManager();

			bool isDeleted(int ptr);
			void setAsDeleted(int ptr);

			static ConnectionManager & instance();
		private:
			static Poco::Mutex globalm;
			Poco::Mutex m;
			typedef std::deque<int> DeletedQueue;
			typedef std::map <int, bool> Deleted;
			DeletedQueue deletedQueue;
			Deleted deleted;
	};

} /* namespace DBus */

#endif /* CONNECTIONMANAGER_H_ */
