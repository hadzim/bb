/*
 * Serialization.h
 *
 *  Created on: Nov 16, 2015
 *      Author: dron
 */

#ifndef SERIALIZATION_H_
#define SERIALIZATION_H_
#include <BB/Rules/Rule.h>
#include <BB/Rules/Rules.h>
#include <ObjectFactory.h>

#include "json/value.h"

#include "TBS/Nullable.h"
#include "BB/RW.h"


namespace BB {


class RulesRW {
	public:
		static Json::Value write(const Rules & v);
		static Rules read(const Json::Value & value);
};

class RuleRW {
	public:
		static Json::Value write(const Rule & v);
		static Rule read(const Json::Value & value);
};

class ActionRW {
	public:
		static Json::Value write(const IAction::Ptr & v);
		static IAction::Ptr read(const Json::Value & value);
};

class ConditionRW {
	public:
		static Json::Value write(const ICondition::Ptr & v);
		static ICondition::Ptr read(const Json::Value & value);
};

class StringRW {
	public:
		static Json::Value write(const std::string & v);
		static std::string read(const Json::Value & value);
};

template <class BaseType, class BaseTypeRW>
class NRW {
	public:
		static Json::Value write(const TBS::Nullable<BaseType> & v){
			if (v.isEmpty()){
				return Json::Value(Json::nullValue);
			}
			return BaseTypeRW::write(v.cref());
		}
		static TBS::Nullable<BaseType> read(const Json::Value & value){
			TBS::Nullable<BaseType> bt;
			if (value.isNull()){
				return bt;
			}
			bt.set(BaseTypeRW::read(value));
			return bt;
		}
};

typedef NRW <std::string, StringRW> NStringRW;

template<class IRWType >
class RWFactory : public ObjectFactory <IRWType, std::string> {
		///create object by its id
	public:
		Poco::SharedPtr<IRWType> create(const std::string & name) {
			IRWType * w;
			w = this->createObject(name);
			if (!w) {
				throw Poco::Exception("No instance " + name + " in factory");
			}
			return w;
		}

		template <class SpecificType>
		bool registerRW(const std::string & id) {
				return this->template registerType<SpecificType>(id);
		}

		///unregister product
		bool unregisterRW(const std::string & id) {
			return this->unregisterObject(id);
		}

		static RWFactory <IRWType> & instance() {
			static Poco::SingletonHolder<RWFactory <IRWType> > sh;
			return *sh.get();
		}
};



class IActionRW {
	public:
		typedef RWFactory <IActionRW> Factory;

		virtual ~IActionRW(){}
		virtual Json::Value write(IAction::Ptr & v) = 0;
		virtual IAction::Ptr read(const Json::Value & value) = 0;

		ABSTRACT_NAMED_OBJECT
};

template <class TAction>
class TActionRW : public IActionRW {
public:
	typedef Poco::SharedPtr<TAction> ActionPtr;

	virtual Json::Value write(IAction::Ptr & v){
		ActionPtr aptr = v.cast<TAction>();
		if (!aptr){
			throw Poco::Exception("Wrong action factory");
		}
		return twrite(aptr);
	}

	virtual Json::Value twrite(ActionPtr & a) = 0;
};


class IConditionRW {
	public:
		typedef RWFactory <IConditionRW> Factory;

		virtual ~IConditionRW(){}
		virtual Json::Value write(ICondition::Ptr & v) = 0;
		virtual ICondition::Ptr read(const Json::Value & value) = 0;

		ABSTRACT_NAMED_OBJECT
};

template <class TCondition>
class TConditionRW : public IConditionRW {
public:
	typedef Poco::SharedPtr<TCondition> ConditionPtr;

	virtual Json::Value write(ICondition::Ptr & v){
		ConditionPtr aptr = v.cast<TCondition>();
		if (!aptr){
			throw Poco::Exception("Wrong condition factory");
		}
		return twrite(aptr);
	}

	virtual Json::Value twrite(ConditionPtr & a) = 0;
};
} /* namespace BB */

#define REGISTER_RW(regClassName, regClass, absClass, nmspc) \
namespace {\
	namespace nmspc {\
			const bool registered = absClass::Factory::instance().registerRW<regClass>(regClassName);\
	}\
}
#define REGISTER_ACTIONRW(regClassName, regClass, nsp) REGISTER_RW(regClassName, regClass, IActionRW, nsp)
#define REGISTER_CONDITIONRW(regClassName, regClass, nsp) REGISTER_RW(regClassName, regClass, IConditionRW, nsp)


#endif /* SERIALIZATION_H_ */
