/*
 * ForgetFactAction.cpp
 *
 *  Created on: Nov 8, 2015
 *      Author: dron
 */

#include <BB/Rules/ForgetFactAction.h>

namespace BB {

ForgetFactAction::ForgetFactAction(Facts & facts, std::string factName) : facts(facts), factName(factName) {

}

ForgetFactAction::~ForgetFactAction() {

}
void ForgetFactAction::perform(){
	facts.erase(factName);
}

} /* namespace BB */
