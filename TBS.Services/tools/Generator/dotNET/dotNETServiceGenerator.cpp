/*
 * JsonServiceGenerator.cpp
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#include "dotNETServiceGenerator.h"
#include "dotNETServiceTemplate.h"

#include "../Xml/Parser.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <Poco/SharedPtr.h>
#include <sstream>
#include "../Json/JsonHelpers.h"

dotNETServiceGenerator::dotNETServiceGenerator(std::string ifile, Info & info){
	Interface i = parseXml(ifile);


	std::string name = "DotNet";

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

dotNETServiceGenerator::~dotNETServiceGenerator() {

}

std::string dotNETServiceGenerator::generateClientMethodSignature(Class & c, std::string cpp){
	return fullClassName(c.namesp, "I" + c.name) + " & " + cpp + c.name + "()";
}



std::string dotNETServiceGenerator::generateHStub(Interface & i, Info & info) {

	string tmp = TEMPLATE_STUB;
	replaceAll(tmp, "<stubname>", info.name);

	string stub_upper = info.name;
	std::transform(stub_upper.begin(), stub_upper.end(), stub_upper.begin(), ::toupper);
	replaceAll(tmp, "<STUBNAME>", stub_upper);
	replaceAll(tmp, "<includes>", "#include \"" + info.getInterfaceHeader() + "\"");


	//std::string fname = jsonp ? "Json" : "Jsonp";
	vector<string> fullnmspc = i.namesp;
	fullnmspc.push_back("DotNet");

	replaceAll(tmp, "<namespaceStart>", generateNamespaceStart(fullnmspc));
	replaceAll(tmp, "<namespaceEnd>", generateNamespaceEnd(fullnmspc));

	//generate procedures
	std::string clientStr;
	std::string privateClientStr;
	std::string serversStr;

	for (Class::List::iterator c = i.classes.begin(); c != i.classes.end(); c++) {
		clientStr.append(generateClientMethodSignature(*c) + ";" + "\n");
		privateClientStr.append(fullClassName(c->namesp, "I" + c->name) + "::Ptr " + variable(c->name) + ";" + "\n");
	}

	replaceAll(tmp, "<clients>", clientStr);
	replaceAll(tmp, "<private>", privateClientStr);
	return tmp;
}

std::string dotNETServiceGenerator::generateCppStub(Interface & i, Info & info) {



	string tmp = TEMPLATE_CPP_STUB;

	replaceAll(tmp, "<includes>", "#include \"" + info.getServiceHeader("DotNet") + "\"\n" + "#include \"" + info.getClientHeader("DotNet") + "\"\n");

	replaceAll(tmp, "<convertors>", JsonHelpers::generateConvertor(i));


	vector<string> fullnmspc = i.namesp;
	fullnmspc.push_back("DotNet");

	replaceAll(tmp, "<namespaceStart>", generateNamespaceStart(fullnmspc));
	replaceAll(tmp, "<namespaceEnd>", generateNamespaceEnd(fullnmspc));

	//generate procedures
	std::stringstream clientMethods;

	for (Class::List::iterator c = i.classes.begin(); c != i.classes.end(); c++) {

		clientMethods << generateClientMethodSignature(*c, "Client::") << "{" << endl;



	    std::vector <std::string> cNamespace = c->namesp;
		cNamespace.push_back("Stub");

		clientMethods << "   if (!" << variable(c->name) << ") {";
		clientMethods << "      " << variable(c->name) << " =  new " << fullClassName(cNamespace, c->name) + "_DotNetClient(params);" << endl;
		clientMethods << "   }";
		clientMethods << "   return *" << variable(c->name) << ";" << endl;
		clientMethods << "}" << endl;

	}

	replaceAll(tmp, "<clientMethods>", clientMethods.str());

	return tmp;
}


