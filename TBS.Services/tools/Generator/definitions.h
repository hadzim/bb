/*
 * definitions.h
 *
 *  Created on: Oct 4, 2013
 *      Author: root
 */

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_
#include <vector>
#include <string>
#include <Poco/File.h>
using namespace std;

#include "TBS/Services/Introspection.h"
using namespace TBS::Services::Introspection;

struct Info {
		string name;
		string includePath;
		string srcPath;
		string docPath;
		string jsPath;
		string relative;



		string getInterfaceDst() {
			Poco::File f(includePath);
			if (!f.exists()){
				f.createDirectories();
			}
			return includePath + name + ".h";
		}
		string getInterfaceSourceDst() {
			return srcPath + name + ".cpp";
		}
		string getServiceHeaderDst(std::string protocol) {
			return includePath + name + "Svc" + "_" + protocol + ".h";
		}
		string getServiceSourceDst(std::string protocol) {
			return srcPath + name + "Svc" + "_" + protocol + ".cpp";
		}
		string getClientDst(std::string protocol) {
			Poco::File f(srcPath + "Client/");
			if (!f.exists()){
				f.createDirectories();
			}
			return srcPath + getClientHeader(protocol);
		}

		string getCSDst() {
			Poco::File f(srcPath);
			if (!f.exists()){
				f.createDirectories();
			}
			return srcPath + "I" + name + ".cs";
		}

		string getJsDst(std::string suffix) {
			Poco::File f(jsPath);
			if (!f.exists()){
				f.createDirectories();
			}
			return jsPath + name + suffix + ".js";
		}

		string getDocDst() {
			Poco::File f(docPath);
			if (!f.exists()){
				f.createDirectories();
			}
			return docPath + name + ".html";
		}

		string getServerDst(std::string protocol) {
			Poco::File f(srcPath + "Server/");
			if (!f.exists()){
				f.createDirectories();
			}
			return srcPath + getServerHeader(protocol);
		}
		string getInterfaceHeader() {
			return relative + name + ".h";
		}
		string getServiceHeader(std::string protocol) {
			return relative + name + "Svc" + "_" + protocol + ".h";
		}
		string getClientHeader(std::string protocol) {
			return "Client/" + name + "_" + protocol + ".h";
		}
		string getServerHeader(std::string protocol) {
			return "Server/" + name + "_" + protocol + ".h";
		}
};


#endif /* DEFINITIONS_H_ */
