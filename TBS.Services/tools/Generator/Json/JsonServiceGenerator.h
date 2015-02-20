/*
 * JsonServiceGenerator.h
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#ifndef JsonServiceGenerator_H_
#define JsonServiceGenerator_H_
#include <string>
#include "../Generator.h"


class JsonServiceGenerator: public Generator {
	public:
		JsonServiceGenerator(std::string interfaceFile, Info & info);
		~JsonServiceGenerator();
	private:
			string generateHStub(Interface & i, Info & in);
			string generateCppStub(Interface & i, Info & in);

			std::string generateClientMethodSignature(Class & c, std::string cpp = "");
			std::string generateServerMethodSignature(Class & c, std::string cpp = "");

		//Interface interface;
};

#endif /* JsonServiceGenerator_H_ */
