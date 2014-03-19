/*
 * JsonServerGenerator.cpp
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#include "JsonServerGenerator.h"
#include "JsonServerTemplate.h"
#include "../Xml/Parser.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <Poco/SharedPtr.h>
#include <sstream>
#include "JsonHelpers.h"

JsonServerGenerator::JsonServerGenerator(std::string ifile, Info & info) {
	Interface i = parseXml(ifile);
	std::string stub = generateStub(i, info);

	{
		std::ofstream of(info.getServerDst("Json").c_str());
		of << stub << std::endl;
	}
}

JsonServerGenerator::~JsonServerGenerator() {

}

std::string JsonServerGenerator::generateStub(Interface & i, Info & info) {

	string tmp = TEMPLATE_SERVER_STUB;
	replaceAll(tmp, "<stubname>", info.name);

	string stub_upper = info.name;
	std::transform(stub_upper.begin(), stub_upper.end(), stub_upper.begin(), ::toupper);
	replaceAll(tmp, "<STUBNAME>", stub_upper);
	replaceAll(tmp, "<includes>", "#include \"" + info.getInterfaceHeader() + "\"");



	std::string s;

	for (Class::List::iterator c = i.classes.begin(); c != i.classes.end(); c++) {
		s.append(generateClass(*c));
	}

	replaceAll(tmp, "<classes>", s);
	return tmp;
}

string JsonServerGenerator::generateClass(Class &c) {
	string tmp = TEMPLATE_CLASS;

	vector<string> fullnmspc = c.namesp;
	fullnmspc.push_back("Stub");

	replaceAll(tmp, "<namespaceStart>", generateNamespaceStart(fullnmspc));
	replaceAll(tmp, "<namespaceEnd>", generateNamespaceEnd(fullnmspc));
	std::string cname = c.name + "_JsonServer";
	replaceAll(tmp, "<className>", cname);
	replaceAll(tmp, "<interfaceName>", c.fullName);
	replaceAll(tmp, "<interface>", fullClassName(c.namesp, "I" + c.name));

	replaceAll(tmp, "<procedureBindings>", this->generateBindings(c));
	replaceAll(tmp, "<procedureDefinitions>", this->generateProcedureDefinitions(c));
	replaceAll(tmp, "<abstractDefinitions>", this->generateAbstractDefinitions(c));

	/*
	 std::string ms;
	 //methods
	 for (Method::List::iterator m = c.methods.begin(); m != c.methods.end(); m++) {
	 ms.append(generateMethod(*m));
	 }
	 replaceAll(tmp, "<methods>", ms);
	 replaceAll(tmp, "<signals>", "//TODO");
	 */
	return tmp;
}

std::string JsonServerGenerator::generateBindings(Class &c) {
	stringstream result;
	string tmp;

	//methods
	for (Method::List::iterator m = c.methods.begin(); m != c.methods.end(); m++) {
		if (m->isVoid()) {
			tmp = TEMPLATE_SERVER_EMPTYMETHODBINDING;
		} else if (m->isReturn()) {
			tmp = TEMPLATE_SERVER_METHODBINDING;
			replaceAll(tmp, "<returnType>", JsonHelpers::toString(JsonHelpers::jsonCast(m->outArguments.front().type)));
		} else {
			std::cout << "More then 2" << std::endl;
			tmp = TEMPLATE_SERVER_EMPTYMETHODBINDING;
		}
		replaceAll(tmp, "<procedureName>", m->name);
		replaceAll(tmp, "<parameterList>", generateBindingParameterlist(*m));
		replaceAll(tmp, "<className>", c.name + "_JsonServer");

		replaceAll(tmp, "<paramType>", "jsonrpc::PARAMS_BY_NAME");

		result << tmp << endl;
	}

	return result.str();
}

std::string JsonServerGenerator::generateBindingParameterlist(Method & m) {

	stringstream parameter;

	for (Argument::List::iterator a = m.inArguments.begin(); a != m.inArguments.end(); a++) {
		parameter << "\"" << a->name << "\"," << JsonHelpers::toString(JsonHelpers::jsonCast(a->type)) << ",";
	}
	/*if (m.outArguments.size() >= 2){
		for (Argument::List::iterator a = m.outArguments.begin(); a != m.outArguments.end(); a++) {
			parameter << "\"" << a->name << "\"," << JsonHelpers::toString(JsonHelpers::jsonCast(a->type)) << ",";
		}
	}*/

	return parameter.str();
}

std::string JsonServerGenerator::generateProcedureDefinitions(Class &c) {
	stringstream result;
	string tmp;
	//methods
	for (Method::List::iterator m = c.methods.begin(); m != c.methods.end(); m++) {

		if (m->isVoid()) {
			tmp = TEMPLATE_SERVER_EMPTYMETHODDEFINITION;
		} else if (m->isReturn()) {
			tmp = TEMPLATE_SERVER_METHODDEFINITION;
				replaceAll(tmp, "<returnType>", returnType(*m));
		} else {
			tmp = TEMPLATE_SERVER_OUTMETHODDEFINITION;
			std::stringstream outParams;
			std::stringstream outParamsAssignment;
			for (Argument::List::iterator a = m->outArguments.begin(); a != m->outArguments.end(); a++) {
				outParams << this->argType(*a) << " tmpvar_" << a->name << ";" <<std::endl;
				outParamsAssignment << "response[\"" + a->name + "\"] = jsonrpc::Convertor::cpp2Json<" + this->argType(*a) + " >(tmpvar_" << a->name << ");" <<std::endl;

			}
			replaceAll(tmp, "<outputParams>", outParams.str());
			replaceAll(tmp, "<responseAssign>",outParamsAssignment.str());
		}

		replaceAll(tmp, "<procedureName>", m->name);
		replaceAll(tmp, "<parameterMapping>", this->generateParameterMapping(*m));
		result << tmp << endl;
	}
	return result.str();
	return "";
}

std::string JsonServerGenerator::generateParameterMapping(Method & m) {
	stringstream parameter;
	string tmp;
	int i = 0;
	for (Argument::List::iterator a = m.inArguments.begin(); a != m.inArguments.end(); a++) {
		tmp = "jsonrpc::Convertor::json2Cpp< " + this->argType(*a) + " >(request[\"" + a->name + "\"])";

		parameter << tmp;
		if (a != --m.inArguments.end() || m.outArguments.size() > 1) {
			parameter << ", ";
		}
		i++;
	}
	if (m.outArguments.size() > 1){
		for (Argument::List::iterator a = m.outArguments.begin(); a != m.outArguments.end(); a++) {
			tmp = "tmpvar_" + a->name;

			parameter << tmp;
			if (a != --m.outArguments.end()) {
				parameter << ", ";
			}
			i++;
		}
	}
	return parameter.str();

}

std::string JsonServerGenerator::generateAbstractDefinitions(Class &c) {

	stringstream result;

	//methods
	for (Method::List::iterator m = c.methods.begin(); m != c.methods.end(); m++) {

		string tmp = TEMPLATE_SERVER_ABSTRACTDEFINITION;

		//set methodname
		replaceAll(tmp, "<methodName>", m->name);
		replaceAll(tmp, "<returnType>", returnType(*m));
		replaceAll(tmp, "<parameters>", methodArgDefinition(*m));
		replaceAll(tmp, "<parameterValues>", methodArgValues(*m));

		/*if (m->outArguments.size() > 1) {
			throw Poco::Exception("cannot convert more then one out param to json client");
		}*/

		if (m->isReturn()) {
			replaceAll(tmp, "<returnStatement>", "return");
		} else {
			replaceAll(tmp, "<returnStatement>", "");
		}

		result << tmp << endl;
	}
	return result.str();

}

/*
 ServerStubGenerator::ServerStubGenerator(const std::string &stubname, const std::string &filename) :
 StubGenerator("Abstract"+stubname + "Server", filename)
 {
 }

 string ServerStubGenerator::generateStub()
 {
 string stub = TEMPLATE_SERVER_STUB;
 replaceAll(stub, "<stubname>", this->stubname);
 replaceAll(stub, "<procedurebindings>", this->generateBindings());
 replaceAll(stub, "<proceduredefinitions>", this->generateProcedureDefinitions());
 replaceAll(stub, "<abstractdefinitions>", this->generateAbstractDefinitions());

 string stub_upper = this->stubname;
 std::transform(stub_upper.begin(), stub_upper.end(), stub_upper.begin(),
 ::toupper);
 replaceAll(stub, "<STUBNAME>", stub_upper);

 return stub;
 }

 string ServerStubGenerator::generateBindings()
 {
 stringstream result;
 string tmp;
 Procedure::Ptr proc;

 for(Procedure::Map::iterator it = this->procedures->begin(); it != this->procedures->end(); it++)
 {
 proc = it->second;
 if(proc->GetProcedureType() == RPC_METHOD)
 {
 tmp = TEMPLATE_SERVER_METHODBINDING;
 }
 else
 {
 tmp = TEMPLATE_SERVER_NOTIFICATIONBINDING;
 }
 replaceAll(tmp, "<procedurename>", normalizeString(proc->GetProcedureName()));
 replaceAll(tmp, "<returntype>", toString(proc->GetReturnType()));
 replaceAll(tmp, "<parameterlist>", generateBindingParameterlist(proc));
 replaceAll(tmp, "<stubname>", this->stubname);

 if(proc->GetParameterDeclarationType() == PARAMS_BY_NAME)
 {
 replaceAll(tmp, "<paramtype>", "jsonrpc::PARAMS_BY_NAME");
 }
 else
 {
 replaceAll(tmp, "<paramtype>", "jsonrpc::PARAMS_BY_POSITION");
 }

 result << tmp << endl;
 }
 return result.str();
 }

 string ServerStubGenerator::generateProcedureDefinitions()
 {
 stringstream result;
 string tmp;
 Procedure::Ptr proc;
 for(Procedure::Map::iterator it = this->procedures->begin(); it != this->procedures->end(); it++)
 {
 proc = it->second;
 if(proc->GetProcedureType() == RPC_METHOD)
 {
 tmp = TEMPLATE_SERVER_METHODDEFINITION;
 }
 else
 {
 tmp = TEMPLATE_SERVER_NOTIFICAITONDEFINITION;
 }
 replaceAll(tmp,"<procedurename>", proc->GetProcedureName());
 replaceAll(tmp,"<parametermapping>", this->generateParameterMapping(proc));
 result << tmp << endl;
 }
 return result.str();
 }

 string ServerStubGenerator::generateAbstractDefinitions()
 {
 stringstream result;
 string tmp;
 Procedure::Ptr proc;

 for(Procedure::Map::iterator it = this->procedures->begin(); it != this->procedures->end(); it++)
 {
 proc = it->second;
 tmp = TEMPLATE_SERVER_ABSTRACTDEFINITION;
 string returntype ="void";
 if(proc->GetProcedureType() == RPC_METHOD)
 {
 returntype = toCppType(proc->GetReturnType());
 }
 replaceAll(tmp, "<returntype>", returntype);
 replaceAll(tmp, "<procedurename>", proc->GetProcedureName());
 replaceAll(tmp, "<parameterlist>", generateParameterDeclarationList(*proc));
 result << tmp;
 }
 return result.str();
 }

 string ServerStubGenerator::generateBindingParameterlist(Procedure::Ptr proc)
 {
 stringstream parameter;
 parameterNameList_t& list = proc->GetParameters();

 for(parameterNameList_t::iterator it2 = list.begin(); it2 != list.end(); it2++)
 {
 parameter << "\"" << it2->first << "\"," << toString(it2->second) << ",";
 }
 return parameter.str();
 }

 string ServerStubGenerator::generateParameterMapping(Procedure::Ptr proc)
 {
 stringstream parameter;
 string tmp;
 parameterNameList_t& params = proc->GetParameters();
 int i=0;
 for(parameterNameList_t::iterator it2 = params.begin(); it2 != params.end(); it2++)
 {
 if(proc->GetParameterDeclarationType() == PARAMS_BY_NAME)
 {
 tmp = "request[\"" + it2->first  + "\"]" + toCppConversion(it2->second);
 }
 else
 {
 stringstream tmp2;
 tmp2 << "request["<< i << "u]" << toCppConversion(it2->second);
 tmp = tmp2.str();
 }
 parameter << tmp;
 if(it2 != --params.end())
 {
 parameter << ", ";
 }
 i++;
 }
 return parameter.str();
 }
 */
