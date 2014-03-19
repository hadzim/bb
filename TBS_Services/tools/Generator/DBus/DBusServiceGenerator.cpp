/*
 * DBusServiceGenerator.cpp
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#include "DBusServiceGenerator.h"
#include "DBusServiceTemplate.h"

#include "../Xml/Parser.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <Poco/SharedPtr.h>
#include <sstream>
#include "DBusHelpers.h"
DBusServiceGenerator::DBusServiceGenerator(std::string ifile, Info & info) {
	Interface i = parseXml(ifile);


	{
		std::string stub = generateHStub(i, info);
		std::ofstream of(info.getServiceHeaderDst("DBus").c_str());
		of << stub << std::endl;
	}
	{
		std::string stub = generateCppStub(i, info);
		std::ofstream of(info.getServiceSourceDst("DBus").c_str());
		of << stub << std::endl;
	}
}

DBusServiceGenerator::~DBusServiceGenerator() {

}

std::string DBusServiceGenerator::generateClientMethodSignature(Class & c, std::string cpp){
	return fullClassName(c.namesp, "I" + c.name) + " & " + cpp + c.name + "()";
}

std::string DBusServiceGenerator::generateServerMethodSignature(Class & c, std::string cpp){
	return "TBS::Services::IServer::Ptr " + cpp + "create" + c.name + "(" + fullClassName(c.namesp, "I" + c.name) + "::Ptr" + " impl)";
}

std::string DBusServiceGenerator::generateHStub(Interface & i, Info & info) {

	string tmp = TEMPLATE_STUB;
	replaceAll(tmp, "<stubname>", info.name);

	string stub_upper = info.name;
	std::transform(stub_upper.begin(), stub_upper.end(), stub_upper.begin(), ::toupper);
	replaceAll(tmp, "<STUBNAME>", stub_upper);
	replaceAll(tmp, "<includes>", "#include \"" + info.getInterfaceHeader() + "\"");

	vector<string> fullnmspc = i.namesp;
	fullnmspc.push_back("DBus");

	replaceAll(tmp, "<namespaceStart>", generateNamespaceStart(fullnmspc));
	replaceAll(tmp, "<namespaceEnd>", generateNamespaceEnd(fullnmspc));

	//generate procedures
	std::string clientStr;
	std::string privateClientStr;
	std::string serversStr;

	for (Class::List::iterator c = i.classes.begin(); c != i.classes.end(); c++) {
		clientStr.append(generateClientMethodSignature(*c) + ";" + "\n");
		privateClientStr.append(fullClassName(c->namesp, "I" + c->name) + "::Ptr " + variable(c->name) + ";" + "\n");
		serversStr.append(generateServerMethodSignature(*c) + ";" + "\n");
	}

	replaceAll(tmp, "<clients>", clientStr);
	replaceAll(tmp, "<private>", privateClientStr);
	replaceAll(tmp, "<servers>", serversStr);
	return tmp;
}

std::string DBusServiceGenerator::generateCppStub(Interface & i, Info & info) {

	string tmp = TEMPLATE_CPP_STUB;

	replaceAll(tmp, "<includes>", "#include \"" + info.getServiceHeader("DBus") + "\"\n" + "#include \"" + info.getClientHeader("DBus") + "\"\n" + "#include \"" + info.getServerHeader("DBus") + "\"\n");

	vector<string> fullnmspc = i.namesp;
	fullnmspc.push_back("DBus");

	replaceAll(tmp, "<namespaceStart>", generateNamespaceStart(fullnmspc));
	replaceAll(tmp, "<namespaceEnd>", generateNamespaceEnd(fullnmspc));

	//generate procedures
	std::stringstream clientMethods;
	std::stringstream serversMethods;

	for (Class::List::iterator c = i.classes.begin(); c != i.classes.end(); c++) {

		clientMethods << generateClientMethodSignature(*c, "Client::") << "{" << endl;



	    std::vector <std::string> cNamespace = c->namesp;
		cNamespace.push_back("Stub");

		clientMethods << "   if (!" << variable(c->name) << ") {";
		clientMethods << "      " << variable(c->name) << " =  new TBS::Services::DBusClientImpl<" << fullClassName(cNamespace, c->name) + "_DBusClient>(comChannelHolder->getConnection());" << endl;
		clientMethods << "   }";
		clientMethods << "   return *" << variable(c->name) << ";" << endl;
		clientMethods << "}" << endl;

		serversMethods << generateServerMethodSignature(*c, "Server::") << "{" << endl;

		serversMethods << "   return new TBS::Services::DBusServerImpl<" << fullClassName(c->namesp, "I" + c->name) << ", " << fullClassName(cNamespace, c->name) << "_DBusServer>(ch, impl);" << std::endl;

		serversMethods << "}" << endl;
	}

	replaceAll(tmp, "<clientMethods>", clientMethods.str());
	replaceAll(tmp, "<serverMethods>", serversMethods.str());

	return tmp;
}


