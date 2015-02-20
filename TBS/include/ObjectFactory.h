#ifndef TBSOBJFACTORY
#define TBSOBJFACTORY

#include <map>
#include <cstddef> // because of NULL definition

#include <TBS/Log.h>

template<class AbstractProduct, typename IdentifierType>
class ObjectFactory {

		typedef AbstractProduct *(*ProductCreator)();

		typedef std::map<IdentifierType, ProductCreator> AssocMap;
		typedef std::pair<IdentifierType, ProductCreator> AssocMapPair;
		AssocMap factoryMap;

		template <class SpecificProduct>
		static AbstractProduct * createObj(){
				return new SpecificProduct();
		}

	public:
		///register product
		bool registerObject(const IdentifierType & id, ProductCreator creator) {
			return this->factoryMap.insert(AssocMapPair(id, creator)).second;
		}
		template <class SpecificProduct>
		bool registerType(const IdentifierType & id) {
			ProductCreator creator = &ObjectFactory<AbstractProduct, IdentifierType>::createObj<SpecificProduct>;
			return this->factoryMap.insert(AssocMapPair(id, creator)).second;
		}
		///unregister product
		bool unregisterObject(const IdentifierType & id) {
			return this->factoryMap.erase(id) == 1;
		}
		///create object by its id
		AbstractProduct * createObject(const IdentifierType & id) {
			typename AssocMap::const_iterator i = this->factoryMap.find(id);
			if (i != this->factoryMap.end()) {
				return (i->second)();
			} else {
				return NULL;
			}
		}

		//iterable interface
		typedef typename AssocMap::iterator iterator;
		iterator begin() {
			return factoryMap.begin();
		}
		iterator end() {
			return factoryMap.end();
		}

		ObjectFactory(){
			LDEBUG("Factory") << "Create factory of " << typeid(AbstractProduct).name() << " addr: " << this << LE;
		}
		~ObjectFactory(){
			LDEBUG("Factory") << "Delete factory of " << typeid(AbstractProduct).name() << " addr: " << this << LE;
		}


};

template<class AbstractProduct, typename p1, typename IdentifierType>
class ObjectFactory1 {

		typedef AbstractProduct *(*ProductCreator)(p1 param1);

		typedef std::map<IdentifierType, ProductCreator> AssocMap;
		typedef std::pair<IdentifierType, ProductCreator> AssocMapPair;
		AssocMap factoryMap;
	protected:
		template <class SpecificProduct>
		static AbstractProduct * createObj(p1 param1){
				return new SpecificProduct(param1);
		}

	public:

		typedef ProductCreator Creator;

		///register product
		bool registerObject(const IdentifierType & id, ProductCreator creator) {
			return this->factoryMap.insert(AssocMapPair(id, creator)).second;
		}
		template <class SpecificProduct>
		bool registerType(const IdentifierType & id) {
			ProductCreator creator = &ObjectFactory1<AbstractProduct, p1, IdentifierType>::createObj<SpecificProduct>;
			return this->factoryMap.insert(AssocMapPair(id, creator)).second;
		}
		///unregister product
		bool unregisterObject(const IdentifierType & id) {
			return this->factoryMap.erase(id) == 1;
		}
		///create object by its id
		AbstractProduct * createObject(const IdentifierType & id, p1 param1) {
			typename AssocMap::const_iterator i = this->factoryMap.find(id);
			if (i != this->factoryMap.end()) {
				return (i->second)(param1);
			} else {
				return NULL;
			}
		}

		//iterable interface
		typedef typename AssocMap::iterator iterator;
		iterator begin() {
			return factoryMap.begin();
		}
		iterator end() {
			return factoryMap.end();
		}

		ObjectFactory1(){
			LDEBUG("Factory") << "Create factory(arg1) of " << typeid(AbstractProduct).name() << " addr: " << this << LE;
		}
		~ObjectFactory1(){
			LDEBUG("Factory") << "Delete factory(arg1) of " << typeid(AbstractProduct).name() << " addr: " << this << LE;
		}


};


template<class AbstractProduct, typename p1, typename p2, typename IdentifierType>
class ObjectFactory2 {

		typedef AbstractProduct *(*ProductCreator)(p1 param1, p2 param2);

		typedef std::map<IdentifierType, ProductCreator> AssocMap;
		typedef std::pair<IdentifierType, ProductCreator> AssocMapPair;
		AssocMap factoryMap;

	protected:
		template <class SpecificProduct>
		static AbstractProduct * createObj(p1 param1, p2 param2){
				return new SpecificProduct(param1, param2);
		}

	public:
		///register product
		bool registerObject(const IdentifierType & id, ProductCreator creator) {
			return this->factoryMap.insert(AssocMapPair(id, creator)).second;
		}
		template <class SpecificProduct>
		bool registerType(const IdentifierType & id) {
			ProductCreator creator = &ObjectFactory2<AbstractProduct, p1, p2, IdentifierType>::createObj<SpecificProduct>;
			return this->factoryMap.insert(AssocMapPair(id, creator)).second;
		}
		///unregister product
		bool unregisterObject(const IdentifierType & id) {
			return this->factoryMap.erase(id) == 1;
		}
		///create object by its id
		AbstractProduct * createObject(const IdentifierType & id, p1 param1, p2 param2) {
			typename AssocMap::const_iterator i = this->factoryMap.find(id);
			if (i != this->factoryMap.end()) {
				return (i->second)(param1, param2);
			} else {
				return NULL;
			}
		}

		//iterable interface
		typedef typename AssocMap::iterator iterator;
		iterator begin() {
			return factoryMap.begin();
		}
		iterator end() {
			return factoryMap.end();
		}

		ObjectFactory2(){
			LDEBUG("Factory") << "Create factory(arg2) of " << typeid(AbstractProduct).name() << " addr: " << this << LE;
		}
		~ObjectFactory2(){
			LDEBUG("Factory") << "Delete factory(arg2) of " << typeid(AbstractProduct).name() << " addr: " << this << LE;
		}

};




#endif
