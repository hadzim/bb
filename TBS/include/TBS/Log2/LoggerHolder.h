#ifndef INCLUDED_UTILITY_LOGGER_HOLDER
#define INCLUDED_UTILITY_LOGGER_HOLDER

#include "LoggerInterface.h"

namespace TBS {
namespace Log2 {

class LoggerHolder
{
public:
	static LoggerInterface::SharedPtr getInstance(void);
	static void setInstance(LoggerInterface::SharedPtr newInstance);
	static void resetToDefault(void);

private:
	static LoggerInterface::SharedPtr instance;
};

} // namespace
} // namespace

#endif // included

