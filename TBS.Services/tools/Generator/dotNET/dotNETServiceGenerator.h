/*
 * JsonServiceGenerator.h
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#ifndef dnJsonServiceGenerator_H_
#define dnJsonServiceGenerator_H_
#include <string>
#include "../Generator.h"


class dotNETServiceGenerator: public Generator {
	public:
		dotNETServiceGenerator(std::string interfaceFile, Info & info);
		~dotNETServiceGenerator();
	private:
			string generateHStub(Interface & i, Info & in);
			string generateCppStub(Interface & i, Info & in);

			std::string generateClientMethodSignature(Class & c, std::string cpp = "");


		//Interface interface;
};

#endif /* JsonServiceGenerator_H_ */
