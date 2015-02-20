#ifndef INCLUDED_UTILITY_LOGGER_INTERFACE
#define INCLUDED_UTILITY_LOGGER_INTERFACE

#include <string>
#include "Poco/SharedPtr.h"

namespace TBS {
namespace Log2 {

class LoggerInterface
{
public:
	enum {
		LCRITICAL = 0,
		LERROR = 1,
		LNOTICE = 2,
		LINFO = 3,
		LDEBUG = 4,
		LTRACE = 5,
	};
	typedef Poco::SharedPtr<LoggerInterface> SharedPtr;

	virtual ~LoggerInterface() {}
	virtual void write(const std::string& name, int severity, int line, const std::string& file, const std::string& func, const std::string& message) = 0;
	virtual int getSeverityTreshold(void) = 0;
};

} // namespace
} // namespace

#endif // included

