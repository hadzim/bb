/*
 * DBusClientGenerator.cpp
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#include "DBusClientGenerator.h"
#include "DBusClientTemplate.h"
#include "../Xml/Parser.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <Poco/SharedPtr.h>
#include <sstream>
#include "DBusHelpers.h"
DBusClientGenerator::DBusClientGenerator(std::string ifile, Info & info) {
	Interface i = parseXml(ifile);
	std::string stub = generateStub(i, info);

	{
		std::ofstream of(info.getClientDst("DBus").c_str());
		of << stub << std::endl;
	}
}

DBusClientGenerator::~DBusClientGenerator() {

}

std::string DBusClientGenerator::generateStub(Interface & i, Info & info) {

	string tmp = TEMPLATE_STUB;
	replaceAll(tmp, "<stubname>", info.name);

	string stub_upper = info.name;
	std::transform(stub_upper.begin(), stub_upper.end(), stub_upper.begin(), ::toupper);
	replaceAll(tmp, "<STUBNAME>", stub_upper);
	replaceAll(tmp, "<includes>", "#include \"" + info.getInterfaceHeader() + "\"");
	//generate procedures
	std::string s;

	for (Class::List::iterator c = i.classes.begin(); c != i.classes.end(); c++) {
		s.append(generateClass(i, *c));
	}

	replaceAll(tmp, "<classes>", s);
	return tmp;
}

string DBusClientGenerator::generateClass(Interface & i, Class &c) {
	string tmp = TEMPLATE_CLASS;

	std::string className = c.name + "_DBusClient";
	vector<string> fullnmspc = c.namesp;
	fullnmspc.push_back("Stub");

	replaceAll(tmp, "<namespaceStart>", generateNamespaceStart(fullnmspc));
	replaceAll(tmp, "<namespaceEnd>", generateNamespaceEnd(fullnmspc));

	replaceAll(tmp, "<interfaceName>", fullClassName(c.namesp, "I" + c.name));
	replaceAll(tmp, "<className>", className);

	replaceAll(tmp, "<dbuspath>", DBusHelpers::dbuspath(i));
	replaceAll(tmp, "<dbusname>", DBusHelpers::dbusname(c));

	std::stringstream construct;

	// generates code to connect all the signal stubs; this is still inside the constructor
	for (Signal::List::iterator si = c.signals.begin(); si != c.signals.end(); ++si) {
		construct << "\t\t" << "connect_signal(" << className << ", " << si->name << ", " << DBusHelpers::stubname(*si) << ");" << endl;
	}
	replaceAll(tmp, "<connectSignals>", construct.str());

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

string DBusClientGenerator::generateMethod(Method & m) {

	stringstream body;

	body << "virtual " << returnType(m) << " " << m.name << "(" << methodArgDefinition(m) << "){";
	body << tab << tab << "try {" << endl;
	body << tab << tab << "::DBus::CallMessage call;" << endl;

	if (!m.inArguments.empty()) {
		body << tab << tab << "::DBus::MessageIter wi = call.writer();" << endl << endl;
	}

	// generate all 'in' arguments for a method body
	int i = 0;
	for (Argument::List::iterator ai = m.inArguments.begin(); ai != m.inArguments.end(); ++ai, ++i) {
		body << tab << tab << "wi << " << ai->name << ";" << endl;
	}
	body << tab << tab << "call.member(\"" << m.name << "\");" << endl;

	body << tab << tab << "::DBus::Message ret = invoke_method (call);" << endl;

	if (!m.outArguments.empty()) {
		body << tab << tab << "::DBus::MessageIter ri = ret.reader();" << endl << endl;
	}

	// generate 'out' values as return if only one existing
	if (m.isReturn()) {
		body << tab << tab << argType(m.outArguments.front()) << " _argout;" << endl;
		body << tab << tab << "ri >> _argout;" << endl;
		body << tab << tab << "return _argout;" << endl;
	} else if (m.outArguments.size() > 1) {
		for (Argument::List::iterator ao = m.outArguments.begin(); ao != m.outArguments.end(); ao++) {
			body << tab << tab << "ri >> " << ao->name << ";" << endl;
		}
	}

	body << tab << tab << "} catch (::DBus::Error & err){" << endl;

	body << tab << tab << tab << "if (strcmp(err.name(), DBUS_ERROR_FAILED) == 0){" << endl;
	body << tab << tab << tab << tab << "throw ::TBS::Services::RuntimeServiceException(err.message());" << endl;
	body << tab << tab << tab << "} else {" << endl;
	body << tab << tab << tab << tab << "throw ::TBS::Services::ChannelServiceException(err.what());" << endl;
	body << tab << tab << tab << "}" << endl;

	body << tab << tab <<"}" << endl;

	body << tab << "}" << endl << endl;

	return body.str();
}

string DBusClientGenerator::generateSignal(Signal & s) {
	stringstream body;

	// generate all the unmarshalers


	body << tab << "void " << DBusHelpers::stubname(s) << "(const ::DBus::SignalMessage &sig)" << "{" << endl;

	if (!s.arguments.empty()) {
		body << tab << tab << "::DBus::MessageIter ri = sig.reader();" << endl << endl;
	}

	for (Argument::List::iterator ai = s.arguments.begin(); ai != s.arguments.end(); ai++){

		body << tab << tab << argType(*ai) << " _" << ai->name << ";" << endl;
		body << tab << tab << "ri >> _" << ai->name << ";" << endl;
	}

	//create signal argument
	body << tab << tab << eventArgType(s) << " sig_arg;" << endl;
	for (Argument::List::iterator ai = s.arguments.begin(); ai != s.arguments.end(); ai++){

		if (s.arguments.size() > 1) {
			body << tab << tab << "sig_arg." << ai->name;
		} else {
			body << tab << tab << "sig_arg";
		}
		body << " = _" << ai->name << ";" << endl;
	}
	body << endl;
	body << tab << tab << "//raise poco event" << endl;
	body << tab << tab << s.name << "(this, sig_arg);" << endl;

	body << tab << "}" << endl;
	return body.str();
}

