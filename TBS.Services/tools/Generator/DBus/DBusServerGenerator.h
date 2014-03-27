/*
 * DBusServerGenerator.h
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#ifndef DBusServerGenerator_H_
#define DBusServerGenerator_H_
#include <string>
#include "../Generator.h"


class DBusServerGenerator: public Generator {
	public:
		DBusServerGenerator(std::string interfaceFile, Info & info);
		~DBusServerGenerator();
	private:
			string generateClass(Interface & i, Class &c);
			string generateStub(Interface & i, Info & in);

			std::string generateBindings(Class &c);
			std::string generateProcedureDefinitions(Class &c);
			std::string generateAbstractDefinitions(Class &c);
			std::string generateBindingParameterlist(Method & m);
			std::string generateParameterMapping(Method & m);
		//Interface interface;
};

#endif /* DBusServerGenerator_H_ */
