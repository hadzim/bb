/*
 * IFact.cpp
 *
 *  Created on: Nov 7, 2015
 *      Author: dron
 */

#include <BB/Rules/IFact.h>

namespace BB {

IFact::IFact(std::string n) : name_(n) {

}

IFact::~IFact() {

}

const std::string IFact::name() const {
	return name_;
}

} /* namespace BB */
