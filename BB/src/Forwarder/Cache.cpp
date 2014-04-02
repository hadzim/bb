/*
 * Cache.cpp
 *
 *  Created on: Feb 22, 2013
 *      Author: root
 */

#include "BB/Forwarder/Cache.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "BB/Sensor/SensorDataRW.h"
#include "Poco/File.h"
#include "Poco/StreamCopier.h"
namespace BB {

	Cache::Cache(std::string path) : path(path) {
		this->fetch();
	}

	Cache::~Cache() {

	}

	void Cache::store(const SensorData & msg){
		Poco::Mutex::ScopedLock l(m);
		std::ofstream fs(path.c_str(), std::ios::app);
		fs << SensorDataRW::write2OneLine(msg) << std::endl;
	}

	Cache::Data Cache::fetch(){
		Poco::Mutex::ScopedLock l(m);

		static char lineData[1024];

		std::list<SensorData> data;

		Poco::File orig(path);
		if (!orig.exists()) return data;

		std::ifstream fs(path.c_str());
		while (fs.good()){
			fs.getline(lineData, 1024);
			if (fs.good()){
				std::string msg(lineData);
				data.push_back(SensorDataRW::read(msg));
			}
		}
		return data;

	}
	void Cache::removeFirst(int count){
		Poco::Mutex::ScopedLock l(m);
		Poco::File orig(path);
		if (!orig.exists()) return;

		std::ifstream fs(path.c_str());
		int counter = 0;
		do {
			char c;
			fs.get(c);
			if (c == '\n'){
				counter++;
			}
		} while (fs.good() && counter < count);

		if (fs.good()){
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

	}

} /* namespace BB */
