/*
 * NodeFactory.cpp
 *
 *  Created on: Dec 10, 2015
 *      Author: dron
 */

#include <BB/Node/NodeFactory.h>

#include "BB/RW.h"

#include "Poco/File.h"
#include <sstream>

namespace BB {

	INodeFactory::~INodeFactory(){

	}

	static std::string filename(std::string name){
		Poco::File dir("settings/factory/");
		if (!dir.exists()){
			dir.createDirectories();
		}
		return dir.path() + "/" + name + ".json";
	}

	DynamicNodeFactory::DynamicNodeFactory(std::string name) : name(name) {

	}

	DynamicNodeFactory::~DynamicNodeFactory() {

	}

	std::string DynamicNodeFactory::getName(){
		return name;
	}

	int DynamicNodeFactory::getCount(std::string name){
		int c = 0;
		try {
			Json::Value v = RW::file2json(filename(name));
			c = v.asInt();
		} catch (Poco::Exception & e){
			setCount(name, c);
		}

		return c;
	}

	void DynamicNodeFactory::setCount(std::string name, int count){
		Json::Value v(count);
		RW::json2file(filename(name), v);
	}

} /* namespace BB */
