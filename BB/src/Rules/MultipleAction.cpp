/*
 * MultipleAction.cpp
 *
 *  Created on: Nov 8, 2015
 *      Author: dron
 */

#include <BB/Rules/MultipleAction.h>

namespace BB {

MultipleAction::MultipleAction(Actions a) :
		actions(a) {

}

MultipleAction::~MultipleAction() {

}

void MultipleAction::addAction(IAction::Ptr action) {
	actions.push_back(action);
}

void MultipleAction::perform() {
	for (auto a : actions) {
		a->perform();
	}
}

} /* namespace BB */
