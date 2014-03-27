/*
 * DBusServiceGenerator.h
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#ifndef DBusServiceGenerator_H_
#define DBusServiceGenerator_H_
#include <string>
#include "../Generator.h"


class DBusServiceGenerator: public Generator {
	public:
		DBusServiceGenerator(std::string interfaceFile, Info & info);
		~DBusServiceGenerator();
	private:
			string generateHStub(Interface & i, Info & in);
			string generateCppStub(Interface & i, Info & in);

			std::string generateClientMethodSignature(Class & c, std::string cpp = "");
			std::string generateServerMethodSignature(Class & c, std::string cpp = "");
		//Interface interface;
};

#endif /* DBusServiceGenerator_H_ */
