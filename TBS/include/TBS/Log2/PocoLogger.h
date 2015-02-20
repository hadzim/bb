#ifndef INCLUDED_UTILITY_POCO_LOGGER
#define INCLUDED_UTILITY_POCO_LOGGER

#include "TBS/Log2/LoggerInterface.h"

namespace TBS {
namespace Log2 {

class PocoLogger: public LoggerInterface
{
public:
	typedef Poco::SharedPtr<PocoLogger> SharedPtr;

	PocoLogger();
	virtual ~PocoLogger();
	virtual void write(const std::string& name, int severity, int line, const std::string& file, const std::string& func, const std::string& message);
	virtual int getSeverityTreshold(void);
};

} // namespace
} // namespace

#endif // included

