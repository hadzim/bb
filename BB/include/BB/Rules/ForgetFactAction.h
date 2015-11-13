/*
 * ForgetFactAction.h
 *
 *  Created on: Nov 8, 2015
 *      Author: dron
 */

#ifndef FORGETFACTACTION_H_
#define FORGETFACTACTION_H_
#include <BB/Rules/Facts.h>
#include <BB/Rules/IAction.h>

namespace BB {

class ForgetFactAction : public IAction {
public:
	ForgetFactAction(Facts & facts, std::string factName);
	virtual ~ForgetFactAction();

	void perform();
private:
	Facts & facts;
	std::string factName;

};

} /* namespace BB */

#endif /* FORGETFACTACTION_H_ */
