/*
 * ReadableEnum.h
 *
 *  Created on: Sep 9, 2013
 *      Author: root
 */

#ifndef READABLEENUM_H_
#define READABLEENUM_H_
#include "TBS.h"
#include <string>
#include <map>

namespace TBS {

		struct TBS_API ReadableEnum {

				std::string toString(const int & val);
				int toInt(const std::string & val);
				bool has(const int & val);

				typedef std::map<int, std::string> Map;

				const Map & values() const {
					return readableEnum;
				}

			protected:
				Map readableEnum;
		};
}

#endif /* READABLEENUM_H_ */
