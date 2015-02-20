/*
 * JsonClientGenerator.cpp
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#include "DocGenerator.h"
#include "DocTemplate.h"
#include "../Xml/Parser.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <Poco/SharedPtr.h>
#include <sstream>
#include <Poco/String.h>

static const char *read_atomic_type_to_string(char t) {
	static struct {
			char type;
			const char *name;
	} atos[] = { { 'y', "uint8" }, { 'b', "bool" }, { 'n', "int16" }, { 'q', "uint16" }, { 'i', "int32" }, { 'u', "uint32" }, { 'x', "int64" }, {
			't', "uint64" }, { 'd', "double" }, { 's', "string" }, {
			'\0', "" } };
	int i;

	for (i = 0; atos[i].type; ++i) {
		if (atos[i].type == t)
			break;
	}
	return atos[i].name;
}

static void _parse_read_signature(const string &signature, string &type, unsigned int &i, bool only_once = false) {

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
			case 'a': {
				switch (signature[++i]) {
					case '{': {
						type += "map < ";
						++i;
						_parse_read_signature(signature, type, i);
						type += " >";

						break;
					}
					case '(': {
						type += "array < struct < ";
						++i;
						_parse_read_signature(signature, type, i);
						type += " > >";

						break;
					}
					default: {
						type += "array < ";
						_parse_read_signature(signature, type, i, true);

						type += " >";

						break;
					}
				}
				break;
			}
			case '(': {
				type += "struct < ";
				++i;

				_parse_read_signature(signature, type, i);

				type += " >";
				break;
			}
			case ')':
			case '}': {
				return;
			}
			default: {
				const char *atom = read_atomic_type_to_string(signature[i]);
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

string read_signature_to_type(const string &signature) {
	string type;
	unsigned int i = 0;
	_parse_read_signature(signature, type, i);
	return type;
}

static std::string readableType(std::string type){
	string otype;
	unsigned int i = 0;
	_parse_read_signature(type, otype, i);
	otype = Poco::replace(otype, "<", "&lt;");
	otype = Poco::replace(otype, ">", "&gt;");
	return otype;
}

std::string readableMethodParams(Method & m) {
	std::stringstream body;
	unsigned int i = 0;
	for (Argument::List::iterator ai = m.inArguments.begin(); ai != m.inArguments.end(); ++ai, ++i) {

		body << readableType(ai->type) << " " << ai->name;

		if ((i + 1 != m.inArguments.size() || m.outArguments.size() > 1))
			body << ", ";
	}

	i = 0;
	if (m.outArguments.size() > 1) {
		for (Argument::List::iterator ai = m.outArguments.begin(); ai != m.outArguments.end(); ++ai, ++i) {
			body << "out " << readableType(ai->type) << " " << ai->name;
			if ((i + 1 != m.outArguments.size())){
						body << ", ";
			}
		}
	}
	return body.str();
}
std::string readableReturnType(Method &method) {
	if (method.outArguments.size() == 0 || method.outArguments.size() > 1) {
		return "void";
	} else {
		return readableType(method.outArguments.front().type);
	}
}

DocGenerator::DocGenerator(std::string ifile, Info & info) {
	Interface i = parseXml(ifile);
	std::string stub = generateStub(i, info);

	{
		std::ofstream of(info.getDocDst().c_str());
		of << stub << std::endl;
	}
}

DocGenerator::~DocGenerator() {

}

std::string DocGenerator::generateStub(Interface & i, Info & info) {

	string tmp = TEMPLATE_STUB;
	replaceAll(tmp, "<serviceName>", info.name);
	replaceAll(tmp, "<serviceComment>", i.comment);
	replaceAll(tmp, "<bootstrap1>", TEMPLATE_BOOTSTRAP1);
	replaceAll(tmp, "<bootstrap2>", TEMPLATE_BOOTSTRAP2);
	replaceAll(tmp, "<bootstrap3>", TEMPLATE_BOOTSTRAP3);

	//generate procedures
	std::string s;

	for (Class::List::iterator c = i.classes.begin(); c != i.classes.end(); c++) {
		s.append(generateClass(*c));
	}

	replaceAll(tmp, "<classes>", s);

	std::string serviceNavigation = "";
	for (Class::List::iterator c = i.classes.begin(); c != i.classes.end(); c++) {
		serviceNavigation.append("<li><a href='#" + c->name + "'>" + c->name + "</a>");
			//methods
				if (!c->methods.empty()){
					serviceNavigation.append("<ul class='nav nav-list'>");
				}
				for (Method::List::iterator m = c->methods.begin(); m != c->methods.end(); m++) {
					serviceNavigation.append("<li><a href='#" + m->name + "'>" + m->name + "</a></li>");
				}
				if (!c->methods.empty()){
					serviceNavigation.append("</ul>");
				}
				serviceNavigation.append("</li>");
	}
	replaceAll(tmp, "<serviceNavigation>", serviceNavigation);

	return tmp;
}

string DocGenerator::generateClass(Class &c) {
	string tmp = TEMPLATE_CLASS;


	replaceAll(tmp, "<interfaceName>", fullClassName(c.namesp,"I" + c.name));
	replaceAll(tmp, "<className>",  c.name);
	replaceAll(tmp, "<classComment>", c.comment);

	std::string ms;
	//methods
	for (Method::List::iterator m = c.methods.begin(); m != c.methods.end(); m++) {
		ms.append(generateMethod(*m));
	}
	replaceAll(tmp, "<methods>", ms);
	replaceAll(tmp, "<signals>", "//TODO");
	replaceAll(tmp, "<address>", "/" + c.fullName);

	return tmp;
}

string DocGenerator::generateMethod(Method & m) {
	string tmp = TEMPLATE_METHOD;

	//set methodname
	replaceAll(tmp, "<methodName>", m.name);
	replaceAll(tmp, "<methodComment>", m.comment);
	replaceAll(tmp, "<returnType>", readableReturnType(m));
	replaceAll(tmp, "<params>", readableMethodParams(m));

	std::string inParams;
	if (m.inArguments.size()) {
		inParams += TEMPLATE_INARGUMENT;

		for (Argument::List::iterator a = m.inArguments.begin(); a != m.inArguments.end(); ++a) {
			std::string par = TEMPLATE_ARGUMENT;
			replaceAll(par, "<type>", readableType(a->type));
			replaceAll(par, "<name>", a->name);
			if (a->comment.length()){
				replaceAll(par, "<comment>", "// " + a->comment);
			} else {
				replaceAll(par, "<comment>", a->comment);
			}
			inParams += par;
		}

	}
	replaceAll(tmp, "<inputParams>", inParams);

	std::string outParams;
	if (m.outArguments.size()) {
		outParams += TEMPLATE_OUTARGUMENT;

		for (Argument::List::iterator a = m.outArguments.begin(); a != m.outArguments.end(); ++a) {
			std::string par = TEMPLATE_ARGUMENT;
			replaceAll(par, "<type>", readableType(a->type));
			replaceAll(par, "<name>", a->name);
			if (a->comment.length()){
				replaceAll(par, "<comment>", "// " + a->comment);
			} else {
				replaceAll(par, "<comment>", a->comment);
			}
			outParams += par;
		}
	}
	replaceAll(tmp, "<outputParams>", outParams);

	replaceAll(tmp, "<address>", "[hostname]:[port]/todo");

	return tmp;
}

