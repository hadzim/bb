/*
 * NamedObject.h
 *
 *  Created on: 21.2.2012
 *      Author: Honza
 */

#ifndef TBS_NAMEDOBJECT_H_
#define TBS_NAMEDOBJECT_H_


#define STATIC_NAMED_OBJECT(name) \
	public:\
		static const std::string & getClassName() { static std::string n = name; return n;} \


#define NAMED_OBJECT(name) \
	public:\
		static const std::string & getClassName() { static std::string n = name; return n;}  \
		virtual const std::string & getName() const { return getClassName(); }

#define NAMED_OBJECT_CONST_STRING(name) \
	public:\
		static const std::string & getClassName()  { return name;}   \
		virtual const std::string & getName() const { static std::string n = name; return n; }


#define ABSTRACT_NAMED_OBJECT \
	public:\
		virtual const std::string & getName() const = 0;


#endif /* NAMEDOBJECT_H_ */
