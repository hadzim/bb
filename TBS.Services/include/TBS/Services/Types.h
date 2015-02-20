/*
 * Services.h
 *
 *  Created on: Oct 7, 2013
 *      Author: root
 */

#ifndef TBS_SERVICESTYPES_H_
#define TBS_SERVICESTYPES_H_
/*
#ifdef GEN_SERVICE_API_
#define GEN_SERVICE_API
#endif
*/
#if defined(_WIN32) && !defined(GEN_SERVICE_API_EMPTY)
#if defined(GEN_SERVICE_EXPORT)
#define GEN_SERVICE_API __declspec(dllexport)
#else
#define GEN_SERVICE_API __declspec(dllimport)
#endif
#endif

#if !defined(GEN_SERVICE_API)
#define GEN_SERVICE_API
#endif

#include "TBS/Nullable.h"

namespace TBS {
	namespace Services {

		struct Invalid {
		};

		template<typename T1, typename T2 = Invalid, typename T3 = Invalid, typename T4 = Invalid, typename T5 = Invalid, typename T6 = Invalid,
				typename T7 = Invalid, typename T8 = Invalid, typename T9 = Invalid, typename T10 = Invalid, typename T11 = Invalid, typename T12 = Invalid,
				typename T13 = Invalid, typename T14 = Invalid, typename T15 = Invalid>
		struct Tuple {
				typedef T1 Type1;
				typedef T2 Type2;
				typedef T3 Type3;
				typedef T4 Type4;
				typedef T5 Type5;
				typedef T6 Type6;
				typedef T7 Type7;
				typedef T8 Type8;
				typedef T9 Type9;
				typedef T10 Type10;
				typedef T11 Type11;
				typedef T12 Type12;
				typedef T13 Type13;
				typedef T14 Type14;
				typedef T15 Type15;

				T1 _1;
				T2 _2;
				T3 _3;
				T4 _4;
				T5 _5;
				T6 _6;
				T7 _7;
				T8 _8;
				T9 _9;
				T10 _10;
				T11 _11;
				T12 _12;
				T13 _13;
				T14 _14;
				T15 _15;
		};

	} // namespace Services

} // namespace TBS

#endif /* SERVICES_H_ */
