/*
 * DBusClientGenerator.h
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#ifndef DBusClientGenerator_H_
#define DBusClientGenerator_H_
#include <string>
#include "../Generator.h"


class DBusClientGenerator: public Generator {
	public:
		DBusClientGenerator(std::string interfaceFile, Info & info);
		~DBusClientGenerator();
	private:
			string generateClass(Interface & i, Class &c);
			string generateMethod(Method & m) ;
			string generateSignal(Signal & s);
			string generateStub(Interface & i, Info & in);
		//Interface interface;
};

#endif /* DBusClientGenerator_H_ */
