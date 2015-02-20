#ifndef INCLUDED_TBS_MAKE_UNIQUE
#define INCLUDED_TBS_MAKE_UNIQUE

#include <memory>

namespace TBS {
	template <typename T, typename... Arguments>
	std::unique_ptr<T> make_unique(Arguments... arguments) { return std::unique_ptr<T>(new T(arguments...)); }
} // namespace

#endif // included
