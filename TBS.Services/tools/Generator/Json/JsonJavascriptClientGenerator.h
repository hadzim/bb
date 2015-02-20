/*
 * JsonJavascriptClientGenerator.h
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#ifndef JsonJavascriptClientGenerator_H_
#define JsonJavascriptClientGenerator_H_
#include <string>
#include "../Generator.h"


class JsonJavascriptClientGenerator: public Generator {
	public:
		JsonJavascriptClientGenerator(std::string interfaceFile, Info & info);
		~JsonJavascriptClientGenerator();
	private:
			string generateClass(Class &c, bool jsonp);
			string generateMethod(Method & m, bool jsonp) ;
			string generateStub(Interface & i, Info & in, bool jsonp);
	private:

		//Interface interface;
};

#endif /* JsonJavascriptClientGenerator_H_ */
