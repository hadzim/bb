/*
 * Page.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: dron
 */

#include "Page.h"

namespace BB {

	Page::Page(std::string name, IScreen::Ptr screen) : name(name), screen(screen) {

	}

	Page::~Page() {

	}

	void Page::show(){

	}
	void Page::hide(){

	}
	std::string Page::getName(){
		return name;
	}

} /* namespace BB */
