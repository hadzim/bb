/*
 * InterfaceGenerator.cpp
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#include "InterfaceGenerator.h"
#include "InterfaceTemplate.h"
#include "Xml/Parser.h"
#include <sstream>
#include <algorithm>
#include <fstream>
#include <iostream>

InterfaceGenerator::InterfaceGenerator(std::string ifile, Info & info) {
	Interface i = parseXml(ifile);
	std::string stub = generateStub(i, info);

	{
		std::ofstream of(info.getInterfaceDst().c_str());
		of << stub << std::endl;
	}

	std::string cppstub = generateCppStub(i, info);

	{
		std::ofstream of(info.getInterfaceSourceDst().c_str());
		of << cppstub << std::endl;
	}
}

InterfaceGenerator::~InterfaceGenerator() {

}

string InterfaceGenerator::generateClass(Class &c) {
	string tmp = TEMPLATE_CLASS;

	replaceAll(tmp, "<namespaceStart>", generateNamespaceStart(c.namesp));
	replaceAll(tmp, "<namespaceEnd>", generateNamespaceEnd(c.namesp));

	replaceAll(tmp, "<className>", "I" + c.name);
	replaceAll(tmp, "<classComment>", "[interface] I" + c.name + ": " + c.comment);
	replaceAll(tmp, "<interfaceName>", c.fullName);

	std::string ms;
	//methods
	for (Method::List::iterator m = c.methods.begin(); m != c.methods.end(); m++) {
		ms.append(generateMethod(*m));
	}
	replaceAll(tmp, "<methods>", ms);

	std::string ss;
	//methods
	for (Signal::List::iterator s = c.signals.begin(); s != c.signals.end(); s++) {
		ss.append(generateSignal(*s));
	}

	replaceAll(tmp, "<signals>", ss);

	return tmp;
}

string InterfaceGenerator::generateStruct(Interface & i, Struct &s) {
	string tmp = TEMPLATE_STRUCTS;

	replaceAll(tmp, "<namespaceStart>", generateNamespaceStart(i.namesp));
	replaceAll(tmp, "<namespaceEnd>", generateNamespaceEnd(i.namesp));

	replaceAll(tmp, "<structName>", s.name);

	std::string ms;
	//methods
	for (Argument::List::iterator m = s.members.begin(); m != s.members.end(); m++) {
		std::string member;
		member = "\t\t" + argType(*m) + " " + m->name + ";\n";
		ms.append(member);
	}
	replaceAll(tmp, "<members>", ms);

	std::cout << "tmp: " << tmp;
	std::cout << "members: " << ms;
	return tmp;
}

string InterfaceGenerator::generateMethod(Method & m) {
	string tmp = TEMPLATE_INTERFACE_METHOD;

	//set methodname
	replaceAll(tmp, "<methodName>", m.name);
	replaceAll(tmp, "<methodComment>", "[method] " + m.name + ": " + m.comment);

	std::stringstream aliases;

	std::stringstream pcom;
	for (Argument::List::iterator a = m.inArguments.begin(); a != m.inArguments.end(); a++) {
		pcom << tab << tab << "  * [in] " << argType(*a) << " " << a->name << ": " << a->comment << endl;
		if (!a->alias.empty()){
			aliases << tab << tab << "typedef " << argType(*a) << " " << a->alias << ";" << endl;
		}
	}
	for (Argument::List::iterator a = m.outArguments.begin(); a != m.outArguments.end(); a++) {
		pcom << tab << tab << "  * [out] " << argType(*a) << " " << a->name << ": " << a->comment << endl;
		if (!a->alias.empty()){
			aliases << tab << tab << "typedef " << argType(*a) << " " << a->alias << ";" << endl;
		}
	}
	replaceAll(tmp, "<paramsComments>", pcom.str());

	replaceAll(tmp, "<aliases>", aliases.str());

	replaceAll(tmp, "<returnType>", returnType(m, true));
	replaceAll(tmp, "<parameters>", methodArgDefinition(m, true));
	return tmp;
}

std::string InterfaceGenerator::generateStub(Interface & i, Info & info) {

	string tmp = TEMPLATE_STUB;
	replaceAll(tmp, "<stubname>", info.name);

	string stub_upper = info.name;
	std::transform(stub_upper.begin(), stub_upper.end(), stub_upper.begin(), ::toupper);
	replaceAll(tmp, "<STUBNAME>", stub_upper);

	{
		//generate procedures
		std::string s;
		for (Class::List::iterator c = i.classes.begin(); c != i.classes.end(); c++) {
			s.append(generateClass(*c));
		}

		replaceAll(tmp, "<classes>", s);
	}
	{
		std::cout << "structs" << std::endl;
			//generate structs
			std::string s;
			for (Struct::Map::iterator c = i.structs.begin(); c != i.structs.end(); c++) {
				std::cout << "struct " << c->second.name << std::endl;
				s.append(generateStruct(i, c->second));
			}

			replaceAll(tmp, "<structs>", s);
		}
	return tmp;
}

static std::string dumpArgument(::TBS::Services::Introspection::Argument & a) {
	return "::TBS::Services::Introspection::Argument(\"" + a.name + "\", \"" + a.type + "\", \"" + a.comment + "\")";
}

string InterfaceGenerator::generateCppStruct(Interface & i, Struct &s){
	string tmp = CPP_CLASS;

	replaceAll(tmp, "<namespaceStart>", generateNamespaceStart(i.namesp));
	replaceAll(tmp, "<namespaceEnd>", generateNamespaceEnd(i.namesp));

	replaceAll(tmp, "<structName>",  s.name);
	return tmp;
}

string InterfaceGenerator::generateCppClass(Class &c) {
	string tmp = CPP_CLASS;

	replaceAll(tmp, "<namespaceStart>", generateNamespaceStart(c.namesp));
	replaceAll(tmp, "<namespaceEnd>", generateNamespaceEnd(c.namesp));

	replaceAll(tmp, "<className>", "I" + c.name);
	replaceAll(tmp, "<interfaceName>", c.fullName);

	std::stringstream intro;

	intro << "::TBS::Services::Introspection::Namespace cnmspc;" << endl;

	for (::TBS::Services::Introspection::Namespace::iterator i = c.namesp.begin(); i != c.namesp.end(); i++) {
		intro << "cnmspc.push_back(\"" << *i << "\");" << endl;
	}

	intro << "::TBS::Services::Introspection::Class c(\"" << c.fullName << "\", \"" << c.name << "\", cnmspc,\"" << c.comment << "\");" << endl;

	//methods
	for (::TBS::Services::Introspection::Method::List::iterator m = c.methods.begin(); m != c.methods.end(); m++) {
		intro << "{" << endl;
		intro << "	::TBS::Services::Introspection::Method m(\"" << m->name << "\", \"" << m->comment << "\");" << endl;

		for (::TBS::Services::Introspection::Argument::List::iterator a = m->inArguments.begin(); a != m->inArguments.end(); a++) {
			intro << "  m.inArguments.push_back(" << dumpArgument(*a) << ");"  << endl;
		}
		for (::TBS::Services::Introspection::Argument::List::iterator a = m->outArguments.begin(); a != m->outArguments.end(); a++) {
			intro << "  m.outArguments.push_back(" << dumpArgument(*a) << ");" << endl;
		}
		intro << "	c.methods.push_back(m);" << endl;
		intro << "}" << endl;
	}
	//signals
	for (::TBS::Services::Introspection::Signal::List::iterator m = c.signals.begin(); m != c.signals.end(); m++) {
		intro << "{" << endl;
		intro << "	::TBS::Services::Introspection::Signal s(\"" << m->name << "\", \"" << m->comment << "\");" << endl;

		for (::TBS::Services::Introspection::Argument::List::iterator a = m->arguments.begin(); a != m->arguments.end(); a++) {
			intro << "  s.arguments.push_back(" << dumpArgument(*a) << ");" << endl;
		}
		intro << "	c.signals.push_back(s);" << endl;
		intro << "}" << endl;
	}
	intro << "return c;" << endl;
	;

	replaceAll(tmp, "<introspection>", intro.str());

	return tmp;
}

string InterfaceGenerator::generateCppStub(Interface & i, Info & info) {
	string tmp = CPP_STUB;
	replaceAll(tmp, "<includes>", "#include \"" + info.getInterfaceHeader() + "\"\n");

	//generate procedures
	std::string s;

	for (Class::List::iterator c = i.classes.begin(); c != i.classes.end(); c++) {
		s.append(generateCppClass(*c));
	}

	replaceAll(tmp, "<classes>", s);

	std::string sts;
	for (Struct::Map::iterator st = i.structs.begin(); st != i.structs.end(); st++) {
		sts.append(generateCppStruct(i, st->second));
	}

	replaceAll(tmp, "<structs>", s);

	return tmp;
}

string InterfaceGenerator::generateSignal(Signal & s) {
	std::string stypedef = "";
	if (s.arguments.size() > 1) {
		stypedef += "struct " + eventArgType(s) + " { \n";
		int i = 0;
		for (Argument::List::iterator a = s.arguments.begin(); a != s.arguments.end(); a++, i++) {
			stypedef += argType(*a) + " " + a->name + ";\n";
		}
		stypedef += "}";
	} else {
		stypedef += "typedef " + this->argType(s.arguments.front()) + " " + eventArgType(s);
	}
	string tmp = TEMPLATE_INTERFACE_SIGNAL;
	replaceAll(tmp, "<signalName>", s.name);
	replaceAll(tmp, "<signalTypeDefinition>", stypedef);
	replaceAll(tmp, "<signalType>", eventArgType(s));
	return tmp;
}
