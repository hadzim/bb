/*
 * Types.h
 *
 *  Created on: Oct 11, 2013
 *      Author: root
 */

#ifndef _NO_DBUS

#ifndef TYPESTBSSERV_H_
#define TYPESTBSSERV_H_

#include <dbus-c++/message.h>
#include <dbus-c++/types.h>
#include "TBS/BLOB.h"
#include "TBS/Services/Types.h"

namespace DBus {

	template <> struct type<TBS::Services::Invalid>
	{
	  static std::string sig()
	  {
	    return "";
	  }
	};

template <
typename T1,
         typename T2,
         typename T3,
         typename T4,
         typename T5,
         typename T6,
         typename T7,
         typename T8,
         typename T9,
         typename T10,
         typename T11,
         typename T12,
         typename T13,
         typename T14,
         typename T15
         >
struct type< TBS::Services::Tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> >
{
  static std::string sig()
  {
	  //std::cout << "SiG tuple "  << std::endl;

    return "("
           + type<T1>::sig()
           + type<T2>::sig()
           + type<T3>::sig()
           + type<T4>::sig()
           + type<T5>::sig()
           + type<T6>::sig()
           + type<T7>::sig()
           + type<T8>::sig()
           + type<T9>::sig()
           + type<T10>::sig()
           + type<T11>::sig()
           + type<T12>::sig()
           + type<T13>::sig()
           + type<T14>::sig()
           + type<T15>::sig()
           + ")";
  }
};

} /* namespace DBus */


inline DBus::MessageIter &operator <<(DBus::MessageIter &iter, const TBS::Services::Invalid &) {
	return iter;
}

template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11,
		typename T12, typename T13, typename T14, typename T15>
DBus::MessageIter &operator <<(DBus::MessageIter &iter,
		const TBS::Services::Tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>& val) {
	DBus::MessageIter sit = iter.new_struct();

	sit << val._1 << val._2 << val._3 << val._4 << val._5 << val._6 << val._7 << val._8 << val._9 << val._10 << val._11 << val._12 << val._13 << val._14
			<< val._15;

	iter.close_container(sit);

	return iter;
}

inline DBus::MessageIter&operator >>(DBus::MessageIter &iter, TBS::Services::Invalid &) {
	return iter;
}

template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11,
		typename T12, typename T13, typename T14, typename T15>
DBus::MessageIter &operator >>(DBus::MessageIter &iter, TBS::Services::Tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>& val) {
	DBus::MessageIter sit = iter.recurse();

	sit >> val._1 >> val._2 >> val._3 >> val._4 >> val._5 >> val._6 >> val._7 >> val._8 >> val._9 >> val._10 >> val._11 >> val._12 >> val._13 >> val._14
			>> val._15;

	return ++iter;
}

#endif /* TYPES_H_ */
#endif
