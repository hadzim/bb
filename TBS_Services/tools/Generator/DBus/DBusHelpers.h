/*
 * DBusHelpers.h
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#ifndef DBusHELPERS_H_
#define DBusHELPERS_H_
#include <string>
#include "../definitions.h"

using namespace std;


class DBusHelpers {
	public:
		//static std::string cppCast(std::string type);
		static string dbuspath(Interface & i);
		static string dbusname(Class & c);
		static string stubname(Signal & s);
		static string stubname(Method & s);
};

#endif /* DBusHELPERS_H_ */
