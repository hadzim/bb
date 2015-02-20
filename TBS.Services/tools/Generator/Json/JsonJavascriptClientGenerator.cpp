/*
 * JsonJavascriptClientGenerator.cpp
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#include "JsonJavascriptClientGenerator.h"
#include "JsonJavascriptClientTemplate.h"
#include "JsonpJavascriptClientTemplate.h"
#include "../Xml/Parser.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <Poco/SharedPtr.h>
#include <sstream>
#include "JsonHelpers.h"
JsonJavascriptClientGenerator::JsonJavascriptClientGenerator(std::string ifile, Info & info) {
	Interface i = parseXml(ifile);



	{
		std::string stub = generateStub(i, info, true);
		std::ofstream of(info.getJsDst(".jsonp").c_str());
		of << stub << std::endl;
	}
	{
		std::string stub = generateStub(i, info, false);
		std::ofstream of(info.getJsDst(".jsonrpc").c_str());
		of << stub << std::endl;
	}
}

JsonJavascriptClientGenerator::~JsonJavascriptClientGenerator() {

}

std::string JsonJavascriptClientGenerator::generateStub(Interface & i, Info & info, bool jsonp) {

	string tmp = TEMPLATE_STUB;

	std::string s;

	for (Class::List::iterator c = i.classes.begin(); c != i.classes.end(); c++) {
		s.append(generateClass(*c, jsonp));
	}

	replaceAll(tmp, "<classes>", s);
	return tmp;
}

string JsonJavascriptClientGenerator::generateClass(Class &c, bool jsonp) {
	string tmp = jsonp ? JSONP_TEMPLATE_CLASS : TEMPLATE_CLASS;

	vector <string> fullnmspc = c.namesp;
	fullnmspc.push_back("Stub");


	replaceAll(tmp, "<interfaceName>", c.fullName);
	replaceAll(tmp, "<className>", c.name);

	std::string ms;
	//methods
	for (Method::List::iterator m = c.methods.begin(); m != c.methods.end(); m++) {
		ms.append(generateMethod(*m, jsonp));
	}
	replaceAll(tmp, "<methods>", ms);

	return tmp;
}

string JsonJavascriptClientGenerator::generateMethod(Method & m, bool jsonp) {
	string tmp = jsonp ? JSONP_TEMPLATE_CLIENT_METHOD : TEMPLATE_CLIENT_METHOD;


	//set methodname
	replaceAll(tmp, "<methodName>", m.name);


	//build parameterlist
	stringstream assignment_string;
	stringstream def_string;
	if (!m.inArguments.empty()){
		if (jsonp){
			assignment_string << "," << std::endl;
		}
		for (Argument::List::iterator a = m.inArguments.begin(); a != m.inArguments.end(); a++){
			if (a != m.inArguments.begin()){
				assignment_string << ", " << std::endl;
				def_string << ", " << std::endl;
			}
			assignment_string << a->name << ": _" << a->name;
			def_string << "_" << a->name;

		}
	}

	replaceAll(tmp, "<inParams>", def_string.str());
	replaceAll(tmp, "<inParamsAssign>", assignment_string.str());

	return tmp;
}

