/*
 * DBusHelpers.cpp
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#include "DBusHelpers.h"
#include <Poco/Exception.h>
#include <Poco/String.h>



string DBusHelpers::stubname(Signal & s) {
	return "_" + s.name + "_sigstub";
}
string DBusHelpers::stubname(Method & s){
	return "_" + s.name + "_mstub";
}

string DBusHelpers::dbuspath(Interface & i) {
	string path = "/com";
	for (vector<string>::iterator n = i.namesp.begin(); n != i.namesp.end(); n++){
		path += "/" + *n;
	}
	return path;
}
string DBusHelpers::dbusname(Class & c) {
	string path = "com";
	for (vector<string>::iterator n = c.namesp.begin(); n != c.namesp.end(); n++){
		path += "." + *n;
	}
	path += "." + c.name;
	return path;
}
