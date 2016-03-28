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

#include <fstream>
#include <sstream>
#include <iostream>
#include "BB/DataRW.h"
#include "Poco/File.h"
#include "Poco/StreamCopier.h"

#include "TBS/Log.h"

namespace BB {

	template<class DataType, class DataTypeRW>
	class TypeCache {
		public:

			typedef std::list<DataType> Data;


			TypeCache(std::string path) :
					path(path), s(0) {
				LERROR("Cache") << "path: " << path << LE;
				this->fetch();
			}

			void store(const DataType & msg) {
				Poco::Mutex::ScopedLock l(m);
				std::ofstream fs(path.c_str(), std::ios::app);
				fs << RW::json2OneLine(DataTypeRW::write(msg)) << std::endl;
				s++;
			}

			Data fetch() {
				Poco::Mutex::ScopedLock l(m);

				static char lineData[2048];

				std::list<DataType> data;

				Poco::File orig(path);
				if (!orig.exists())
					return data;

				std::ifstream fs(path.c_str());
				while (fs.good()) {
					fs.getline(lineData, 2048);
					if (fs.good()) {
						std::string msg(lineData);
						if (data.empty()){
							LERROR("Cache") << "first cached in " << path << ": " << msg << LE;
						}
						data.push_back(DataTypeRW::read(RW::string2json(msg)));
					}
				}
				s = data.size();
				return data;

			}
			void removeFirst(int count) {
				Poco::Mutex::ScopedLock l(m);
				Poco::File orig(path);
				if (!orig.exists())
					return;

				std::ifstream fs(path.c_str());
				int counter = 0;
				do {
					char c;
					fs.get(c);
					if (c == '\n') {
						counter++;
					}
				} while (fs.good() && counter < count);

				if (fs.good()) {
					std::string tmpname = path + ".tmp";
					{
						std::ofstream tmp(tmpname.c_str());
						Poco::StreamCopier::copyStream(fs, tmp);
					}
					fs.close();
					Poco::File tfile(tmpname);
					tfile.moveTo(path);
				} else {
					fs.close();
					orig.remove();
				}

				s -= count;
				if (s < 0) s = 0;

			}

			int size(){
				return s;
			}
		private:
			Poco::Mutex m;
			std::string path;
			int s;
	};

} /* namespace BB */
#endif /* CACHE_H_ */
