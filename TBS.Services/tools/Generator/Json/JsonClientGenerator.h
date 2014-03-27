/*
 * JsonClientGenerator.h
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#ifndef JsonClientGenerator_H_
#define JsonClientGenerator_H_
#include <string>
#include "../Generator.h"


class JsonClientGenerator: public Generator {
	public:
		JsonClientGenerator(std::string interfaceFile, Info & info);
		~JsonClientGenerator();
	private:
			string generateClass(Class &c);
			string generateMethod(Method & m) ;
			string generateStub(Interface & i, Info & in);
		//Interface interface;
};

#endif /* JsonClientGenerator_H_ */
