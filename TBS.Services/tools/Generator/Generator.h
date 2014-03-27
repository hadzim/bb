/*
 * Generator.h
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#ifndef GENERATOR_H_
#define GENERATOR_H_
#include <string>
#include "definitions.h"

using namespace std;

class Generator {
	public:
		Generator();
		~Generator();
	public:
		static string fullClassName(const vector<string> & namespaces, std::string name);
		static std::string argType( Argument & a, bool useAliases = false);
	protected:
		static void replaceAll(std::string& text, const std::string& fnd, const std::string& rep);

		static const std::string tab;

		string generateNamespaceStart(vector <string> & namespaces);
		string generateNamespaceEnd(vector <string> & namespaces);




		std::string returnType(Method &method, bool useAliases = false);

		std::string eventArgType(Signal &s);

		std::string methodArgValues(Method & m, std::string prefix = "");
		std::string signalArgValues(Signal & m, std::string prefix = "");
		std::string methodArgDefinition(Method & m, bool useAliases = false);

		std::string signalArgDefinition(Signal & m);

		std::string variable(std::string className);


};

#endif /* GENERATOR_H_ */
