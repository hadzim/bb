/*
 * Generator.cpp
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#include "Generator.h"

#include <sstream>
#include <iostream>
#include <stdexcept>
#include <Poco/Net/HTTPResponse.h>
#include <algorithm>

using namespace std;

const std::string Generator::tab = "\t";

Generator::Generator() {
	// TODO Auto-generated constructor stub

}

Generator::~Generator() {
	// TODO Auto-generated destructor stub
}

std::string Generator::eventArgType(Signal &s) {
	return s.name + "Arg";
}

void Generator::replaceAll(string &text, const string &fnd, const string &rep) {
	std::string newstr = Poco::replace(text, fnd, rep);
	text = newstr;
}

string Generator::fullClassName(const vector<string> & namespaces, std::string name) {
	std::stringstream s;
	for (vector<string>::const_iterator i = namespaces.begin(); i != namespaces.end(); i++) {
		s << *i << "::";
	}
	s << name;
	return s.str();
}

string Generator::fullNamepace(const vector<string> & namespaces) {
	std::stringstream s;
	for (vector<string>::const_iterator i = namespaces.begin(); i != namespaces.end(); i++) {
		s << *i;
		if (i + 1 != namespaces.end()) {
			s<< "::";
		}
	}
	return s.str();
}

string Generator::generateNamespaceStart(vector<string> & namespaces) {
	std::stringstream s;
	int tabCnt = 0;
	for (vector<string>::iterator i = namespaces.begin(); i != namespaces.end(); i++) {
		for (int st = 0; st < tabCnt; st++) {
			s << "\t";
		}
		tabCnt++;
		s << "namespace " << *i << " { " << endl;
	}
	return s.str();
}
string Generator::generateNamespaceEnd(vector<string> & namespaces) {
	std::stringstream s;
	for (vector<string>::iterator i = namespaces.begin(); i != namespaces.end(); i++) {
		s << " } " << endl;
	}
	return s.str();
}

std::string Generator::variable(std::string className) {
	std::string data = className;
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	return data + "_";
}

static const char *atomic_type_to_string(char t) {
	static struct {
			char type;
			const char *name;
	} atos[] = { { 'y', "uint8_t" }, { 'b', "bool" }, { 'n', "int16_t" }, { 'q', "uint16_t" }, { 'i', "int32_t" }, { 'u', "uint32_t" }, { 'x', "int64_t" }, {
			't', "uint64_t" }, { 'd', "double" }, { 's', "std::string" }, { 'o', "::DBus::Path" }, { 'g', "::DBus::Signature" }, { 'v', "::DBus::Variant" }, {
			'\0', "" } };
	int i;

	for (i = 0; atos[i].type; ++i) {
		if (atos[i].type == t)
			break;
	}
	return atos[i].name;
}

static void _parse_signature(const string &signature, string &type, unsigned int &i, bool only_once = false) {
	/*cout << "signature: " << signature << endl;
	 cout << "type: " << type << endl;
	 cout << "i: " << i << ", signature[i]: " << signature[i] << endl;*/

	for (; i < signature.length(); ++i) {
		switch (signature[i]) {
			case '[': {

				int start = i + 1;
				do {
					i++;
				} while(signature[i] != ']');
				std::string structname = signature.substr(start, i - start);
				type += structname;
				std::cout << "STRUCTNAME: " << structname << std::endl;
				break;
			}
			case 'N': {
				type += "TBS::Nullable< ";
				++i;
				_parse_signature(signature, type, i, true);
				type += " >";
				break;
			}
			case 'a': {
				switch (signature[++i]) {
					case '{': {
						type += "std::map< ";
						++i;
						_parse_signature(signature, type, i);
						type += " >";

						break;
					}
					case '(': {
						type += "std::vector< TBS::Services::Tuple< ";
						++i;
						_parse_signature(signature, type, i);
						type += " > >";

						break;
					}
					default: {
						type += "std::vector< ";
						_parse_signature(signature, type, i, true);

						type += " >";

						break;
					}
				}
				break;
			}
			case '(': {
				type += "TBS::Services::Tuple< ";
				++i;

				_parse_signature(signature, type, i);

				type += " >";
				break;
			}
			case ')':
			case '}': {
				return;
			}
			default: {
				const char *atom = atomic_type_to_string(signature[i]);
				if (!atom) {
					cerr << "invalid signature" << endl;
					throw runtime_error("cannot parse");
				}
				type += atom;

				break;
			}
		}

		if (only_once)
			return;

		if (i + 1 < signature.length() && signature[i + 1] != ')' && signature[i + 1] != '}') {
			type += ", ";
		}
	}
}

string signature_to_type(const string &signature) {
	string type;
	unsigned int i = 0;
	_parse_signature(signature, type, i);
	return type;
}

std::string Generator::signalArgValues(Signal & m, std::string prefix) {
	std::stringstream body;
	unsigned int i = 0;
	for (Argument::List::iterator ai = m.arguments.begin(); ai != m.arguments.end(); ++ai, ++i) {

		body << prefix << ai->name;

		if ((i + 1 != m.arguments.size()))
			body << ", ";
	}
	return body.str();
}

std::string Generator::methodArgValues(Method & m, std::string prefix) {
	std::stringstream body;
	unsigned int i = 0;
	for (Argument::List::iterator ai = m.inArguments.begin(); ai != m.inArguments.end(); ++ai, ++i) {

		body << prefix << ai->name;

		if ((i + 1 != m.inArguments.size() || m.outArguments.size() > 1))
			body << ", ";
	}

	if (m.outArguments.size() > 1) {
		// generate all 'out' arguments for a method signature
		unsigned int j = 0;
		for (Argument::List::iterator ao = m.outArguments.begin(); ao != m.outArguments.end(); ++ao, ++j) {

			body << prefix << ao->name;

			if (j + 1 != m.outArguments.size())
				body << ", ";
		}
	}
	return body.str();
}

std::string Generator::signalArgDefinition(Signal & m) {
	std::stringstream body;
	unsigned int i = 0;
	for (Argument::List::iterator ai = m.arguments.begin(); ai != m.arguments.end(); ++ai, ++i) {
		body << "const " << argType(*ai) << " & " << ai->name;
		if ((i + 1 != m.arguments.size()))
			body << ", ";
	}
	return body.str();

}

std::string Generator::methodArgDefinition(Method & m, bool useAliases) {
	std::stringstream body;
	unsigned int i = 0;
	for (Argument::List::iterator ai = m.inArguments.begin(); ai != m.inArguments.end(); ++ai, ++i) {

		body << "const " << argType(*ai, useAliases) << " & " << ai->name;

		if ((i + 1 != m.inArguments.size() || m.outArguments.size() > 1))
			body << ", ";
	}

	if (m.outArguments.size() > 1) {
		// generate all 'out' arguments for a method signature
		unsigned int j = 0;
		for (Argument::List::iterator ao = m.outArguments.begin(); ao != m.outArguments.end(); ++ao, ++j) {

			body << argType(*ao, useAliases) << " & " << ao->name;

			if (j + 1 != m.outArguments.size())
				body << ", ";
		}
	}
	return body.str();
}


std::string Generator::argType(Argument & a, bool useAliases) {
	if (useAliases && !a.alias.empty()){
		return a.alias;
	}
	return signature_to_type(a.type);
}

std::string Generator::returnType(Method &method, bool useAliases) {
	if (method.outArguments.size() == 0 || method.outArguments.size() >= 2) {
		return "void";
	} else {
		return argType(method.outArguments.front(), useAliases);
	}
}

