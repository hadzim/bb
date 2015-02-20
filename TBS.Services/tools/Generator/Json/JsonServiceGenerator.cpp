/*
 * JsonServiceGenerator.cpp
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#include "JsonServiceGenerator.h"
#include "JsonServiceTemplate.h"

#include "../Xml/Parser.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <Poco/SharedPtr.h>
#include <sstream>
#include "JsonHelpers.h"
JsonServiceGenerator::JsonServiceGenerator(std::string ifile, Info & info) {
	Interface i = parseXml(ifile);

	std::string name = "Json";

	{
		std::string stub = generateHStub(i, info);
		std::ofstream of(info.getServiceHeaderDst(name).c_str());
		of << stub << std::endl;
	}
	{
		std::string stub = generateCppStub(i, info);
		std::ofstream of(info.getServiceSourceDst(name).c_str());
		of << stub << std::endl;
	}
}

JsonServiceGenerator::~JsonServiceGenerator() {

}

std::string JsonServiceGenerator::generateClientMethodSignature(Class & c, std::string cpp){
	return fullClassName(c.namesp, "I" + c.name) + " & " + cpp + c.name + "()";
}

std::string JsonServiceGenerator::generateServerMethodSignature(Class & c, std::string cpp){
	std::cout << "create server " << c.name << std::endl;
	return "TBS::Services::IServer::Ptr " + cpp + "create" + c.name + "(" + fullClassName(c.namesp, "I" + c.name) + "::Ptr" + " impl)";
}

std::string JsonServiceGenerator::generateHStub(Interface & i, Info & info) {

	string tmp = TEMPLATE_STUB;
	replaceAll(tmp, "<stubname>", info.name);

	string stub_upper = info.name;
	std::transform(stub_upper.begin(), stub_upper.end(), stub_upper.begin(), ::toupper);
	replaceAll(tmp, "<STUBNAME>", stub_upper);
	replaceAll(tmp, "<includes>", "#include \"" + info.getInterfaceHeader() + "\"");


	//std::string fname = jsonp ? "Json" : "Jsonp";
	vector<string> fullnmspc = i.namesp;
	fullnmspc.push_back("Json");

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

std::string JsonServiceGenerator::generateCppStub(Interface & i, Info & info) {

	//std::string channel = jsonp ? "TBS::Services::JsonpCommChannelHolder" : "TBS::Services::JsonCommChannelHolder";

	string tmp = TEMPLATE_CPP_STUB;

	std::string aname = "Json";

	replaceAll(tmp, "<includes>", "#include \"" + info.getServiceHeader(aname) + "\"\n" + "#include \"" + info.getClientHeader("Json") + "\"\n" + "#include \"" + info.getServerHeader("Json") + "\"\n");

	replaceAll(tmp, "<convertors>", JsonHelpers::generateConvertor(i));



	vector<string> fullnmspc = i.namesp;
	fullnmspc.push_back("Json");

	replaceAll(tmp, "<namespaceStart>", generateNamespaceStart(fullnmspc));
	replaceAll(tmp, "<namespaceEnd>", generateNamespaceEnd(fullnmspc));

	//generate procedures
	std::stringstream clientMethods;
	std::stringstream serialMethods;
	std::stringstream serversMethods;

	for (Class::List::iterator c = i.classes.begin(); c != i.classes.end(); c++) {

		clientMethods << generateClientMethodSignature(*c, "Client::") << "{" << endl;



	    std::vector <std::string> cNamespace = c->namesp;
		cNamespace.push_back("Stub");

		clientMethods << "   if (!" << variable(c->name) << ") {\n\n";

		clientMethods << "      if (httpCh.isSet()) {\n";
		clientMethods << "         " << variable(c->name) << " =  new " << fullClassName(cNamespace, c->name) + "_JsonClient(TBS::Services::createClientConnector(" + fullClassName(c->namesp, "I" + c->name) +"::name(), httpCh.cref()));" << endl;
		clientMethods << "      }\n\n";

		clientMethods << "      if (serialCh.isSet()) {\n";
		clientMethods << "         " << variable(c->name) << " =  new " << fullClassName(cNamespace, c->name) + "_JsonClient(TBS::Services::createClientConnector(" + fullClassName(c->namesp, "I" + c->name) +"::name(), serialCh.cref()));" << endl;
		clientMethods << "      }\n\n";


		clientMethods << "   }\n";
		clientMethods << "   return *" << variable(c->name) << ";" << endl;
		clientMethods << "}" << endl;

		serversMethods << generateServerMethodSignature(*c, "Server::") << "{" << endl;


		serversMethods << "   return new TBS::Services::JsonServerImpl<" << fullClassName(c->namesp, "I" + c->name) << ", " << fullClassName(cNamespace, c->name) << "_JsonServer>(this->channel.cast<TBS::Services::AJsonCommChannelHolder>()->getInterface(), impl);" << std::endl;

		serversMethods << "}" << endl;
	}

	replaceAll(tmp, "<clientMethods>", clientMethods.str());
	replaceAll(tmp, "<serverMethods>", serversMethods.str());

	return tmp;
}


