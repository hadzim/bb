#ifndef INCLUDED_TBS_JSON_VALUE_GETTER
#define INCLUDED_TBS_JSON_VALUE_GETTER

#include "json/value.h"
#include <memory>
#include "TBS/TBS.h"

namespace TBS {

	/**
		Very simple class to provide fast, read-only access to Json tree, based on paths.
	*/
	class TBS_API JsonValueGetter {
	public:
		JsonValueGetter();
		JsonValueGetter(std::shared_ptr<const Json::Value> root, const std::string& path);

		/// throws std::runtime_error when name not in address or value is not convertible to given type
		const Json::Value& get(const std::string& name, Json::ValueType check) const;

		/// does not check anything, adds '.' to beginning of name if necessary
		JsonValueGetter subtree(const std::string& name) const;

	private:
		std::shared_ptr<const Json::Value> root;
		std::string path;
	};

} // namespace

#endif // included

