/*
 * JsonClientGenerator.cpp
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#include "dotNETClientGenerator.h"
#include "dotNETClientTemplate.h"
#include "../Xml/Parser.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <Poco/SharedPtr.h>
#include <sstream>
#include "../Json/JsonHelpers.h"

dotNETClientGenerator::dotNETClientGenerator(std::string ifile, Info & info) {
	Interface i = parseXml(ifile);
	std::string stub = generateStub(i, info);

	{
		std::ofstream of(info.getClientDst("DotNet").c_str());
		of << stub << std::endl;
	}
}

dotNETClientGenerator::~dotNETClientGenerator() {

}

std::string dotNETClientGenerator::generateStub(Interface & i, Info & info) {

	string tmp = TEMPLATE_STUB;
	replaceAll(tmp, "<stubname>", info.name);

	string stub_upper = info.name;
	std::transform(stub_upper.begin(), stub_upper.end(), stub_upper.begin(), ::toupper);
	replaceAll(tmp, "<STUBNAME>", stub_upper);
	replaceAll(tmp, "<includes>", "#include \"" + info.getInterfaceHeader() + "\"");

	//generate procedures
	std::string s;

	for (Class::List::iterator c = i.classes.begin(); c != i.classes.end(); c++) {
		s.append(generateClass(*c));
	}

	replaceAll(tmp, "<classes>", s);
	return tmp;
}

string dotNETClientGenerator::generateClass(Class &c) {
	string tmp = TEMPLATE_CLASS;

	vector <string> fullnmspc = c.namesp;
	fullnmspc.push_back("Stub");

	replaceAll(tmp, "<namespaceStart>", generateNamespaceStart(fullnmspc));
	replaceAll(tmp, "<namespaceEnd>", generateNamespaceEnd(fullnmspc));

	replaceAll(tmp, "<interfaceName>", fullClassName(c.namesp,"I" + c.name));
	std::string cname = c.name + "_DotNetClient";
	replaceAll(tmp, "<className>", cname);

	std::string ms;
	//methods
	for (Method::List::iterator m = c.methods.begin(); m != c.methods.end(); m++) {
		ms.append(generateMethod(c, *m));
	}
	replaceAll(tmp, "<methods>", ms);

	return tmp;
}

string dotNETClientGenerator::generateMethod(Class &c, Method & m) {
	string tmp = TEMPLATE_CLIENT_METHOD;


	//set methodname
	replaceAll(tmp, "<methodName>", m.name);
	replaceAll(tmp, "<className>", c.name);
	replaceAll(tmp, "<returnType>", returnType(m));
	replaceAll(tmp, "<parameters>", methodArgDefinition(m));

	//build parameterlist
	stringstream assignment_string;

	/*
	url << "apid=" << apid;
	url << "&";

	url << "timestamp=" << timestamp;
	url << "&";

	url << "eventtype=" << eventtype;
*/
	if (!m.inArguments.empty()){
		for (Argument::List::iterator a = m.inArguments.begin(); a != m.inArguments.end(); a++){
			if (a != m.inArguments.begin()){
				assignment_string << "url << \"&\";" << std::endl;
			}
			assignment_string << "url << \"" << a->name << "=\" << " << a->name << ";" << std::endl;; //= jsonrpc::Convertor::cpp2Json< " << argType(*a) << " >(" << a->name << "); " << endl;
		}
	} else {
		//assignment_string << "pIn = ::Json::nullValue;";
	}

	replaceAll(tmp, "<parameterAssign>", assignment_string.str());


	if (m.isReturn()){
		replaceAll(tmp, "<returnStatement>", "return jsonrpc::Convertor::json2Cpp<" + returnType(m) + " >(this->client.sendRequest(url.str()));");
	} else {
		//parse out arguments

		if (m.outArguments.size() > 1) {
			throw Poco::Exception("dot net more then one argument");
			/*stringstream assignment_outstring;
			assignment_outstring << "::Json::Value pOut = this->client->CallMethod(\"" << m.name << "\", pIn);" << endl<< endl;
			for (Argument::List::iterator a = m.outArguments.begin(); a != m.outArguments.end(); a++){
				assignment_outstring << a->name << " = " << "jsonrpc::Convertor::json2Cpp<" << argType(*a) + " >(" << "pOut[\"" << a->name << "\"]); " << endl;
			}
			replaceAll(tmp, "<returnStatement>", assignment_outstring.str());*/
		} else {
			replaceAll(tmp, "<returnStatement>", "this->client.sendRequest(url.str());");
		}
		//replaceAll(tmp, "<returnStatement>", "this->client->CallNotification(\"" + m.name + "\", p);");
	}
	return tmp;
}

