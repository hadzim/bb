/*
 * dotNETInterfaceGenerator.cpp
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#include "dotNETInterfaceGenerator.h"
#include "dotNETInterfaceTemplate.h"
#include <sstream>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "../Xml/Parser.h"
#include "Poco/Exception.h"
#include <Poco/String.h>

dotNETInterfaceGenerator::dotNETInterfaceGenerator(std::string ifile, Info & info) {
	Interface i = parseXml(ifile);
	std::string stub = generateStub(i, info);

	{
		std::ofstream of(info.getCSDst().c_str());
		of << stub << std::endl;
	}


}

dotNETInterfaceGenerator::~dotNETInterfaceGenerator() {

}

std::string dotNETInterfaceGenerator::methodArgUrl(Method & m){
	std::stringstream body;
		unsigned int i = 0;
		for (Argument::List::iterator ai = m.inArguments.begin(); ai != m.inArguments.end(); ++ai, ++i) {

			body << "" << ai->name << "={" << ai->name << "}";

			if ((i + 1 != m.inArguments.size() || m.outArguments.size() > 1))
				body << "&";
		}
		return body.str();
}

std::string dotNETInterfaceGenerator::methodArgDefinitionCs(Method & m) {
	std::stringstream body;
	unsigned int i = 0;
	for (Argument::List::iterator ai = m.inArguments.begin(); ai != m.inArguments.end(); ++ai, ++i) {

		body << "" << argTypeCs(*ai) << " " << ai->name;

		if ((i + 1 != m.inArguments.size() || m.outArguments.size() > 1))
			body << ", ";
	}

	if (m.outArguments.size() > 1) {
		throw Poco::Exception("Cannot create .net with more output args");
	}
	return body.str();
}


string dotNETInterfaceGenerator::generateClass(Class &c) {
	string tmp = TEMPLATE_CLASS;

	replaceAll(tmp, "<namespaceStart>", "namespace TBS.Services {");
	replaceAll(tmp, "<namespaceEnd>", "}");

	replaceAll(tmp, "<className>", "I" + c.name);
	//replaceAll(tmp, "<classComment>", "[interface] I" + c.name + ": " + c.comment);
	replaceAll(tmp, "<interfaceName>", c.fullName);

	std::string ms;
	//methods
	for (Method::List::iterator m = c.methods.begin(); m != c.methods.end(); m++) {
		ms.append(generateMethod(*m));
	}
	replaceAll(tmp, "<methods>", ms);

	return tmp;
}

string dotNETInterfaceGenerator::generateMethod(Method & m) {
	string tmp = TEMPLATE_INTERFACE_METHOD;

	//set methodname
	replaceAll(tmp, "<methodName>", m.name);
	replaceAll(tmp, "<methodComment>", "[method] " + m.name + ": " + m.comment);

	std::stringstream aliases;


	replaceAll(tmp, "<urlParameters>", methodArgUrl(m));
	replaceAll(tmp, "<returnType>", returnTypeCs(m));
	replaceAll(tmp, "<parameters>", methodArgDefinitionCs(m));
	return tmp;
}

std::string dotNETInterfaceGenerator::generateStub(Interface & i, Info & info) {

	string tmp = TEMPLATE_STUB;
	replaceAll(tmp, "<stubname>", info.name);

	string stub_upper = info.name;
	std::transform(stub_upper.begin(), stub_upper.end(), stub_upper.begin(), ::toupper);
	replaceAll(tmp, "<STUBNAME>", stub_upper);

	//generate procedures
	std::string s;

	for (Class::List::iterator c = i.classes.begin(); c != i.classes.end(); c++) {
		s.append(generateClass(*c));
	}

	replaceAll(tmp, "<classes>", s);
	return tmp;
}

std::string dotNETInterfaceGenerator::argTypeCs(Argument & a) {
	std::string s = argType(a, false);
	Poco::replaceInPlace(s, "std::", "");
	Poco::replaceInPlace(s, "vector<", "List<");
	Poco::replaceInPlace(s, "map<", "Dictionary<");
	Poco::replaceInPlace(s, "int32_t", "int");
	return s;
}

std::string dotNETInterfaceGenerator::returnTypeCs(Method &method) {
	if (method.outArguments.size() == 0 || method.outArguments.size() > 2) {
		return "void";
	} else {
		return argTypeCs(method.outArguments.front());
	}
}



