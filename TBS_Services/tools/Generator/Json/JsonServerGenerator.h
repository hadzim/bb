/*
 * JsonServerGenerator.h
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#ifndef JsonServerGenerator_H_
#define JsonServerGenerator_H_
#include <string>
#include "../Generator.h"


class JsonServerGenerator: public Generator {
	public:
		JsonServerGenerator(std::string interfaceFile, Info & info);
		~JsonServerGenerator();
	private:
			string generateClass(Class &c);
			string generateStub(Interface & i, Info & in);

			std::string generateBindings(Class &c);
			std::string generateProcedureDefinitions(Class &c);
			std::string generateAbstractDefinitions(Class &c);
			std::string generateBindingParameterlist(Method & m);
			std::string generateParameterMapping(Method & m);
		//Interface interface;
};

#endif /* JsonServerGenerator_H_ */
