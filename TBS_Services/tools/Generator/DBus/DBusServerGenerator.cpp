/*
 * DBusServerGenerator.cpp
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#include "DBusServerGenerator.h"
#include "DBusServerTemplate.h"
#include "../Xml/Parser.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <Poco/SharedPtr.h>
#include <sstream>
#include "DBusHelpers.h"
DBusServerGenerator::DBusServerGenerator(std::string ifile, Info & info) {
	Interface i = parseXml(ifile);
	std::string stub = generateStub(i, info);

	{
		std::ofstream of(info.getServerDst("DBus").c_str());
		of << stub << std::endl;
	}
}

DBusServerGenerator::~DBusServerGenerator() {

}

std::string DBusServerGenerator::generateStub(Interface & i, Info & info) {

	string tmp = TEMPLATE_SERVER_STUB;
	replaceAll(tmp, "<stubname>", info.name);

	string stub_upper = info.name;
	std::transform(stub_upper.begin(), stub_upper.end(), stub_upper.begin(), ::toupper);
	replaceAll(tmp, "<STUBNAME>", stub_upper);
	replaceAll(tmp, "<includes>", "#include \"" + info.getInterfaceHeader() + "\"");

	std::string s;

	for (Class::List::iterator c = i.classes.begin(); c != i.classes.end(); c++) {
		s.append(generateClass(i, *c));
	}

	replaceAll(tmp, "<classes>", s);
	return tmp;
}

string DBusServerGenerator::generateClass(Interface & i, Class &c) {
	string tmp = TEMPLATE_CLASS;

	vector<string> fullnmspc = c.namesp;
	fullnmspc.push_back("Stub");

	replaceAll(tmp, "<namespaceStart>", generateNamespaceStart(fullnmspc));
	replaceAll(tmp, "<namespaceEnd>", generateNamespaceEnd(fullnmspc));
	std::string cname = c.name + "_DBusServer";
	replaceAll(tmp, "<className>", cname);
	replaceAll(tmp, "<interfaceName>", c.fullName);
	replaceAll(tmp, "<interface>", fullClassName(c.namesp, "I" + c.name));

	replaceAll(tmp, "<dbuspath>", DBusHelpers::dbuspath(i));
	replaceAll(tmp, "<dbusname>", DBusHelpers::dbusname(c));

	{
		std::stringstream body;
		std::stringstream dbody;

		//generate code to bind events
		for (Signal::List::iterator si = c.signals.begin(); si != c.signals.end(); ++si) {
			body << tab << tab << "impl->" << si->name << " += Poco::delegate(this, &" << cname << "::handler" << si->name << ");" << endl;
			dbody << tab << tab << "impl->" << si->name << " -= Poco::delegate(this, &" << cname << "::handler" << si->name << ");" << endl;
		}

		// generate code to register all methods
		for (Method::List::iterator m = c.methods.begin(); m != c.methods.end(); m++) {
			body << tab << tab << "register_method(" << cname << ", " << m->name << ", " << DBusHelpers::stubname(*m) << ");" << endl;
		}
		replaceAll(tmp, "<binding>", body.str());
		replaceAll(tmp, "<unbinding>", dbody.str());
	}
	{
		std::stringstream body;
		// generate the signals code
		for (Signal::List::iterator si = c.signals.begin(); si != c.signals.end(); ++si) {
			{
				body << tab << "void " << si->name << "(" << signalArgDefinition(*si) << "){" << endl;
				body << tab << tab << "::DBus::SignalMessage sig(\"" << si->name << "\");" << endl;

				// generate the signal body
				if (!si->arguments.empty()) {
					body << tab << tab << "::DBus::MessageIter wi = sig.writer();" << endl;
					for (Argument::List::iterator a = si->arguments.begin(); a != si->arguments.end(); ++a) {
						body << tab << tab << "wi << " << a->name << ";" << endl;
					}
				}
				// emit the signal in method body
				body << tab << tab << "emit_signal(sig);" << endl;
				body << tab << "}" << endl;
			}
		}
		replaceAll(tmp, "<signals>", body.str());
	}
	{
		std::stringstream body;
		// generate the unmarshalers
		for (Method::List::iterator m = c.methods.begin(); m != c.methods.end(); m++) {

			body << tab << "::DBus::Message " << DBusHelpers::stubname(*m) << "(const ::DBus::CallMessage &call)" << "{" << endl;

			body << tab << tab << "try {" << endl;

			if (m->inArguments.empty()){
				body << tab << tab << "call.reader();" << endl;
			} else {
				body << tab << tab << "::DBus::MessageIter ri = call.reader();" << endl;
			}


			// generate the 'in' variables
			for (Argument::List::iterator ai = m->inArguments.begin(); ai != m->inArguments.end(); ++ai) {
				body << tab << tab << argType(*ai) << " _" << ai->name << ";" << endl;
				body << "ri >> _" << ai->name << ";" << endl;
			}



			// generate 'out' variables
			if (!m->outArguments.empty()) {
				for (Argument::List::iterator ao = m->outArguments.begin(); ao != m->outArguments.end(); ++ao) {

					body << tab << tab << argType(*ao) << " _" << ao->name << (m->isReturn() ? " = " : ";\n");
				}
			}

			body << "impl->" << m->name << "(" << methodArgValues(*m, "_") << ");" << endl;
			body << tab << tab << "::DBus::ReturnMessage reply(call);" << endl;

			if (!m->outArguments.empty()) {
				body << tab << tab << "::DBus::MessageIter wi = reply.writer();" << endl;
				for (Argument::List::iterator ao = m->outArguments.begin(); ao != m->outArguments.end(); ++ao) {
					body << tab << tab << "wi << " << " _" << ao->name << ";" << endl;
				}
			}

			body << tab << tab << "return reply;" << endl;

			body << tab << tab << "} catch (Poco::Exception &ex){" << endl;
			body << tab << tab << tab << "return ::DBus::ErrorMessage(call, DBUS_ERROR_FAILED, ex.message().c_str());" << endl;
			body << tab << tab << "} catch (std::exception & ex){" << endl;
			body << tab << tab << tab << "return ::DBus::ErrorMessage(call, DBUS_ERROR_FAILED, ex.what());" << endl;
			body << tab << tab << "} " << endl;

			body << tab << "}" << endl;

		}
		replaceAll(tmp, "<methods>", body.str());
	}
	{
		std::stringstream body;

		for (Signal::List::iterator si = c.signals.begin(); si != c.signals.end(); ++si) {
			body << tab << tab << "void " << "handler" << si->name << "(" << fullClassName(c.namesp, "I" + c.name) << "::" << eventArgType(*si) << " & arg){" << endl;

			body << tab << tab << tab << "this->" << si->name << "(";

			if (si->arguments.size() > 1) {
				body << signalArgValues(*si, "arg.");
			} else if (si->arguments.size() == 1) {
				body << "arg";
			}

			body << ");" << endl;
			body << tab << tab << "}" << endl;
		}

		replaceAll(tmp, "<handlers>", body.str());
	}

	return tmp;
}
