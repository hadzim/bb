/*
 * MultipleAction.h
 *
 *  Created on: Nov 8, 2015
 *      Author: dron
 */

#ifndef MULTIPLEACTION_H_
#define MULTIPLEACTION_H_
#include <BB/Rules/IAction.h>
#include <vector>

namespace BB {

class MultipleAction : public IAction {
public:
	typedef std::vector <IAction::Ptr> Actions;

	typedef Poco::SharedPtr <MultipleAction> Ptr;

	MultipleAction(Actions a = Actions());
	virtual ~MultipleAction();

	void addAction(IAction::Ptr action);

	void perform();
private:
	Actions actions;
};

} /* namespace BB */

#endif /* MULTIPLEACTION_H_ */
