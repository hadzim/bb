/*
 * JsonHelpers.cpp
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#include "JsonHelpers.h"
#include <Poco/Exception.h>

#include "../Generator.h"
#include <iostream>
#include <sstream>

std::string JsonHelpers::generateConvertor(TBS::Services::Introspection::Interface & i) {
	std::stringstream str;
	str << "namespace jsonrpc {";
	for (TBS::Services::Introspection::Struct::Map::iterator it = i.structs.begin(); it != i.structs.end(); it++){
		str << generateConvertor(i, it->second);
	}
	str << "}";
	return str.str();
}
std::string JsonHelpers::generateConvertor(TBS::Services::Introspection::Interface & i, TBS::Services::Introspection::Struct & s) {
	std::stringstream str;
	std::string fullname = Generator::fullClassName(i.namesp, s.name);
	str << "template<>" << std::endl;
	str << "class InternalConvertor<" << fullname <<"> { " << std::endl;
	str << "	public:" << std::endl;
	str << "		static " << fullname << " json2Cpp(const Json::Value & val) {" << std::endl;
	str << "		   " << fullname << " var;" << std::endl;
	for (TBS::Services::Introspection::Argument::List::iterator m = s.members.begin(); m != s.members.end(); m++){
		str << "		   		var." << m->name << " =  Convertor::json2Cpp< " << Generator::argType(*m, false) << " > (val[\"" << m->name << "\"]);" << std::endl;
	}
	str << "			return var;" << std::endl;
	str << "		}" << std::endl;
	str << "		static Json::Value cpp2Json(const " << fullname << " & val) {" << std::endl;
	str << "		    Json::Value retval(Json::objectValue);" << std::endl;
	for (TBS::Services::Introspection::Argument::List::iterator m = s.members.begin(); m != s.members.end(); m++){
		str << "		    retval[\"" << m->name << "\"] = Convertor::cpp2Json< " << Generator::argType(*m, false) << " >(val." << m->name << ");"  << std::endl;
	}
	str << "			return retval;" << std::endl;
	str << "		}" << std::endl;
	str << "};" << std::endl;
	return str.str();

}

jsonrpc::jsontype_t JsonHelpers::jsonCast(std::string type) {
	if (type == "i") {
		return jsonrpc::JSON_INTEGER;
	}
	if (type == "s") {
		return jsonrpc::JSON_STRING;
	}
	if (type == "as") {
		return jsonrpc::JSON_ARRAY;
	}
	if (type == "b") {
		return jsonrpc::JSON_BOOLEAN;
	}
	if (type[0] == 'a') {
		return jsonrpc::JSON_ARRAY;
	}
	if (type[0] == '(') {
		return jsonrpc::JSON_ARRAY;
	}
	if (type[0] == '[') {
			return jsonrpc::JSON_OBJECT;
		}
	throw Poco::Exception("jsonCast Unknown type " + type);
}
std::string JsonHelpers::toString(jsonrpc::jsontype_t type) {
	std::string result;
	switch (type) {
		case jsonrpc::JSON_BOOLEAN:
			result = "jsonrpc::JSON_BOOLEAN";
			break;
		case jsonrpc::JSON_INTEGER:
			result = "jsonrpc::JSON_INTEGER";
			break;
		case jsonrpc::JSON_REAL:
			result = "jsonrpc::JSON_REAL";
			break;
		case jsonrpc::JSON_STRING:
			result = "jsonrpc::JSON_STRING";
			break;
		case jsonrpc::JSON_OBJECT:
			result = "jsonrpc::JSON_OBJECT";
			break;
		case jsonrpc::JSON_ARRAY:
			result = "jsonrpc::JSON_ARRAY";
			break;
		case jsonrpc::JSON_NULL:
			result = "jsonrpc::JSON_NULL";
	}
	return result;
}
