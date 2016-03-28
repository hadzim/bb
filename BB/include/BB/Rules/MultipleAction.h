/*
 * MultipleAction.h
 *
 *  Created on: Nov 8, 2015
 *      Author: dron
 */

#ifndef MULTIPLEACTION_H_
#define MULTIPLEACTION_H_
#include <BB/Rules/IAction.h>
#include <BB/Rules/Serialization.h>
#include <vector>

namespace BB {

class MultipleActionRW;

class MultipleAction : public IAction {
public:
	typedef std::vector <IAction::Ptr> Actions;

	typedef Poco::SharedPtr <MultipleAction> Ptr;

	MultipleAction(Actions a = Actions());
	virtual ~MultipleAction();

	void addAction(IAction::Ptr action);

	void perform();
	void init(ActionParams params);

	NAMED_OBJECT("Multiple")

	friend class MultipleActionRW;
private:
	Actions actions;
};

class MultipleActionRW : public TActionRW <MultipleAction> {
public:
	virtual Json::Value twrite(ActionPtr & a);
	virtual IAction::Ptr read(const Json::Value & value);

	NAMED_OBJECT(MultipleAction::getClassName())
};

} /* namespace BB */

#endif /* MULTIPLEACTION_H_ */
