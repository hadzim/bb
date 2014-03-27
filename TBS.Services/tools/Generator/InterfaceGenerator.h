/*
 * InterfaceGenerator.h
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#ifndef INTERFACEGENERATOR_H_
#define INTERFACEGENERATOR_H_

#include "definitions.h"
#include "Generator.h"

class InterfaceGenerator: public Generator {
	public:
		InterfaceGenerator(std::string ifile, Info & info);
		virtual ~InterfaceGenerator();
	private:
		string generateStruct(Interface & i, Struct &s);
		string generateCppStruct(Interface & i, Struct &s);
		string generateClass(Class &c);
		string generateMethod(Method & m) ;
		string generateStub(Interface & i, Info & in);
		string generateCppClass(Class &c);
		string generateCppStub(Interface & i, Info & in);
		string generateSignal(Signal & s);


};
#endif /* INTERFACEGENERATOR_H_ */
