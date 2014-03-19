/*
 * utils.h
 *
 *  Created on: 10.10.2011
 *      Author: Honza
 */

#ifndef STRUCTURES_UTILS_H_
#define STRUCTURES_UTILS_H_

#include "TBS/TBS.h"

namespace TBS {

	template<typename def, typename inner = typename def::type>
	class safe_enum: public def {
		public:
			typedef typename def::type type;
		private:
			inner val;

		public:

			safe_enum(type v) :
					val(v) {
			}
			inner underlying() const {
				return val;
			}

			bool operator ==(const safe_enum & s) const {
				return this->val == s.val;
			}
			bool operator !=(const safe_enum & s) const {
				return this->val != s.val;
			}
			bool operator <(const safe_enum & s) const {
				return this->val < s.val;
			}
			bool operator <=(const safe_enum & s) const {
				return this->val <= s.val;
			}
			bool operator >(const safe_enum & s) const {
				return this->val > s.val;
			}
			bool operator >=(const safe_enum & s) const {
				return this->val >= s.val;
			}
	};

}
#endif /* UTILS_H_ */
