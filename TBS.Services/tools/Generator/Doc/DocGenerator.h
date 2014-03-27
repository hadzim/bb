/*
 * JsonClientGenerator.h
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#ifndef DocGenerator_H_
#define DocGenerator_H_
#include <string>
#include "../Generator.h"

class DocGenerator: public Generator {
	public:
		DocGenerator(std::string interfaceFile, Info & info);
		~DocGenerator();
	private:
		string generateClass(Class &c);
		string generateMethod(Method & m);
		string generateStub(Interface & i, Info & in);
		//Interface interface;
};

#endif /* JsonClientGenerator_H_ */
