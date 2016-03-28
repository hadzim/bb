/*
 * IAction.cpp
 *
 *  Created on: Nov 7, 2015
 *      Author: dron
 */

#include <BB/Rules/IAction.h>

namespace BB {

IAction::IAction() {

}

IAction::~IAction() {

}
void IAction::init(ActionParams params){
	this->params = params;
}

} /* namespace BB */
