/*
 * Parser.cpp
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#include "Parser.h"

#include <Poco/StringTokenizer.h>
#include "xml.h"
#include <fstream>

void parseName(string input, string & name, vector<string> & namesp) {
	Poco::StringTokenizer tk(input, ".");
	for (Poco::StringTokenizer::Iterator i = tk.begin(); i != tk.end(); i++) {
		namesp.push_back(*i);
	}
	name = namesp.back();
	namesp.pop_back();
}

void parseName(string input, vector<string> & namesp) {
	Poco::StringTokenizer tk(input, ".");
	for (Poco::StringTokenizer::Iterator i = tk.begin(); i != tk.end(); i++) {
		namesp.push_back(*i);
	}
}
std::string argName(Xml::Node &arg, int num) {

	std::stringstream mbody;

	string arg_name = arg.get("name");
	if (arg_name.length())
		mbody << arg_name;
	else
		mbody << "arg" << num;

	return mbody.str();
}

void parseArgs(Xml::Nodes & args, Argument::List & argOut) {

	unsigned int i = 0;

	for (Xml::Nodes::iterator ai = args.begin(); ai != args.end(); ++ai, ++i) {
		Xml::Node &arg = **ai;
		Argument a(argName(arg, i), arg.get("type"),  arg.get("comment"));
		a.alias = arg.get("alias");
		argOut.push_back(a);
	}
}

Interface parseXml(std::string file) {
	ifstream xmlfile(file.c_str());

	if (xmlfile.bad()) {
		cerr << "unable to open file " << file << endl;
		throw runtime_error("cannot open file");
	}

	Xml::Document doc;

	try {
		xmlfile >> doc;
		//cout << doc.to_xml();
	} catch (Xml::Error &e) {
		cerr << "error to parse " << file << ": " << e.what() << endl;
		throw;
	}


	Namespace in;

	Xml::Node &root = *(doc.root);

	parseName(root.get("name"), in);

	Interface interface(in, root.get("comment"));

	Xml::Nodes structs = root["struct"];

		// iterate over all interface definitions
	for (Xml::Nodes::iterator i = structs.begin(); i != structs.end(); ++i) {
		Xml::Node &xstr = **i;

		Struct s;
		s.name = xstr.get("name");
		Xml::Nodes xargs = xstr["arg"];
		parseArgs(xargs, s.members);

		//std::cout << "struct " << s.name << " size: " << s.members.size() << std::endl;
		interface.structs[s.name] = s;
		interface.structsIndexes.push_back(s.name);
	}

	Xml::Nodes xinterfaces = root["interface"];

	// iterate over all interface definitions
	for (Xml::Nodes::iterator i = xinterfaces.begin(); i != xinterfaces.end(); ++i) {

		Xml::Node &xiface = **i;
		Xml::Nodes xmethods = xiface["method"];
		Xml::Nodes xsignals = xiface["signal"];
		Xml::Nodes xms;
		xms.insert(xms.end(), xmethods.begin(), xmethods.end());
		xms.insert(xms.end(), xsignals.begin(), xsignals.end());

		// gets the name of a interface: <interface name="XYZ">
		string ifacename = xiface.get("name");
		string cname;
		Namespace cns;
		parseName(ifacename, cname, cns);
		Class c(ifacename, cname, cns, xiface.get("comment"));

		std::cout << "class " << c.name << " nmspc: " << c.namesp.size() << " ifacename " << ifacename << std::endl;

		for (Xml::Nodes::iterator xmi = xmethods.begin(); xmi != xmethods.end(); ++xmi) {
			Xml::Node &xmethod = **xmi;
			Xml::Nodes xargs = xmethod["arg"];
			Xml::Nodes xargs_in = xargs.select("direction", "in");
			Xml::Nodes xargs_out = xargs.select("direction", "out");

			Method m( xmethod.get("name"),  xmethod.get("comment"));

			parseArgs(xargs_in, m.inArguments);
			parseArgs(xargs_out, m.outArguments);

			c.methods.push_back(m);
		}

		for (Xml::Nodes::iterator xmi = xsignals.begin(); xmi != xsignals.end(); ++xmi) {
			Xml::Node &xmethod = **xmi;
			Xml::Nodes xargs = xmethod["arg"];

			Signal s(xmethod.get("name"), xmethod.get("comment"));
			parseArgs(xargs, s.arguments);
			c.signals.push_back(s);
		}

		interface.classes.push_back(c);
	}
	return interface;
}
