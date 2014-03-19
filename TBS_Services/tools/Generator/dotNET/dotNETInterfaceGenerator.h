/*
 * dotNETInterfaceGenerator.h
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#ifndef INTERFACEGENERATORdotNET_H_
#define INTERFACEGENERATORdotNET_H_

#include "../definitions.h"
#include "../Generator.h"

class dotNETInterfaceGenerator: public Generator {
	public:
		dotNETInterfaceGenerator(std::string ifile, Info & info);
		virtual ~dotNETInterfaceGenerator();
	private:
		string generateClass(Class &c);
		string generateMethod(Method & m) ;
		string generateStub(Interface & i, Info & in);

		std::string methodArgDefinitionCs(Method & m);
		std::string methodArgUrl(Method & m);

		std::string argTypeCs( Argument & a);
		std::string returnTypeCs(Method &method);




};
#endif /* INTERFACEGENERATOR_H_ */
