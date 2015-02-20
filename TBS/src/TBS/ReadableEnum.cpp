/*
 * ReadableEnum.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: dron
 */

#include "TBS/ReadableEnum.h"
#include "Poco/Exception.h"
#include <Poco/NumberFormatter.h>


namespace TBS {
	std::string ReadableEnum::toString(const int & val) {
		Map::iterator i = readableEnum.find(val);
		if (i == readableEnum.end()) {
			throw Poco::Exception("Unknown enumeration value " + Poco::NumberFormatter::format(val));
		}
		return i->second;
	}
	int ReadableEnum::toInt(const std::string & val) {
		for (Map::iterator i = readableEnum.begin(); i != readableEnum.end(); i++) {
			if (i->second == val) {
				return i->first;
			}
		}
		throw Poco::Exception("Unknown enumeration value: " + val);
	}

	bool ReadableEnum::has(const int & val){
		Map::iterator i = readableEnum.find(val);
		return  i != readableEnum.end();
	}


}  // namespace TBS
