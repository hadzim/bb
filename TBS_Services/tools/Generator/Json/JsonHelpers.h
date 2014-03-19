/*
 * JsonHelpers.h
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#ifndef JSONHELPERS_H_
#define JSONHELPERS_H_
#include <string>
#include "jsonrpc/specification.h"

#include "TBS/Services/Introspection.h"

class JsonHelpers {
	public:
		//static std::string cppCast(std::string type);
		static jsonrpc::jsontype_t jsonCast(std::string type);
		static std::string toString(jsonrpc::jsontype_t t);

		static std::string generateConvertor(TBS::Services::Introspection::Interface & i);
		static std::string generateConvertor(TBS::Services::Introspection::Interface & i, TBS::Services::Introspection::Struct & s);
};

#endif /* JSONHELPERS_H_ */
