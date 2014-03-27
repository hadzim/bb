/*
 * dotNETClientGenerator.h
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#ifndef dotNETClientGenerator_H_
#define dotNETClientGenerator_H_
#include <string>
#include "../Generator.h"


class dotNETClientGenerator: public Generator {
	public:
		dotNETClientGenerator(std::string interfaceFile, Info & info);
		~dotNETClientGenerator();
	private:
			string generateClass(Class &c);
			string generateMethod(Class &c, Method & m) ;
			string generateStub(Interface & i, Info & in);

};

#endif /* dotNETClientGenerator_H_ */
