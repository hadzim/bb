/*
 * TemplatedContainer.h
 *
 *  Created on: May 20, 2011
 *      Author: dron
 */

#ifndef TEMPLATEDCONTAINER_H_
#define TEMPLATEDCONTAINER_H_


#include <map>
#include <sstream>
#include <iosfwd>
#include <iostream>
#include "Poco/Exception.h"


namespace TBS {


/**
 * Holdes type safe collection
 * used for States, Contexts, ...
 * */
template <class HoldedClassType>
class TypeSafeContainer {
	/**
	 * data stored in std::map
	 * */
	typedef typename std::map <std::string, HoldedClassType *> StorageType;
	/**
	 * iterator
	 * */
	typedef typename StorageType::iterator StorageTypeIterator;

	/**
	 * internal storage
	 * */
	StorageType container;

public:
	/**
	 * method adds instance to internal storage
	 * takes ownership
	 * */
	void addInstance(HoldedClassType * newInstance){
		if (!newInstance){
			throw Poco::Exception("instance is null");
		}
		container.insert(std::pair <std::string, HoldedClassType *>(newInstance->getName(), newInstance));
	}



	/**
	 * get instance by name and type (name should be registered to factory with type)
	 * */
	HoldedClassType & getInstance(const std::string & instanceName){
		//create new if not exists
		if (container.find(instanceName) == container.end()){
			std::stringstream ms;
			ms << "getExistingInstance: '" << instanceName << "'  does not exists in container";
			throw Poco::Exception(ms.str());
		}
		//get instance from storage
		return *container[instanceName];

	}
	/**
	 * get instance by name and type (name should be registered to factory with type)
	 * */
	template <class InstanceType>
	InstanceType & getInstance(){
		try {
			//get instance from storage
			return dynamic_cast<InstanceType &>(this->getInstance(InstanceType::getClassName()));
		} catch (std::exception & e){
			std::stringstream ms;
			ms << "getExistingInstance<>: '" << InstanceType::getClassName() << "'  failed (" << e.what() << ")";
			throw Poco::Exception(ms.str());
		}
	}

	bool hasInstance(const std::string & instanceName){
		return container.find(instanceName) != container.end();
	}

public:
	//iterable interface
	typedef StorageTypeIterator iterator;
	iterator begin(){ return container.begin(); }
	iterator end(){	return container.end();}

public:
	/**
	 * Clear context data
	 * */
	~TypeSafeContainer(){
		for (StorageTypeIterator i = begin(); i != end(); i++){
			if (i->second){
				delete (i->second);
				i->second = NULL;
				//LOG_STREAM_INFO << i->first << LOG_STREAM_END
			}
		}
		container.clear();
		//LOG_STREAM_INFO << "done" << LOG_STREAM_END
	}



};

}


#endif /* TEMPLATEDCONTAINER_H_ */
