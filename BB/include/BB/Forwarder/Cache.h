/*
 * Cache.h
 *
 *  Created on: Feb 22, 2013
 *      Author: root
 */

#ifndef CACHE_H_
#define CACHE_H_
#include <string>
#include "Poco/Mutex.h"
#include <list>
#include "BB/Sensor/SensorData.h"

namespace BB {

	class Cache {
		public:
			Cache(std::string name);
			virtual ~Cache();

			void store(const SensorData & m);

			typedef std::list<SensorData> Data;
			Data fetch();
			void removeFirst(int count);
		private:
			Poco::Mutex m;
			std::string path;
	};

} /* namespace BB */
#endif /* CACHE_H_ */
