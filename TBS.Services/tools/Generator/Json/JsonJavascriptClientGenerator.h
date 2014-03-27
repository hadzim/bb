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
		JsonJavascriptClientGenerator(std::string interfaceFile, Info & info, bool jsonp = false);
		~JsonJavascriptClientGenerator();
	private:
			string generateClass(Class &c);
			string generateMethod(Method & m) ;
			string generateStub(Interface & i, Info & in);
	private:
			bool jsonp ;
		//Interface interface;
};

#endif /* JsonJavascriptClientGenerator_H_ */
