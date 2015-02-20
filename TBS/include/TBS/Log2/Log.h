#ifndef INCLUDED_UTILITY_LOG
#define INCLUDED_UTILITY_LOG

#include <sstream>
#include "LoggerInterface.h"
#include "TBS/TBS.h"

// helper macro, don't use in the code
#define LOG_GENERIC__(SEVERITY) \
	TBS::Log2::Log( \
		TBS::Log2::LoggerInterface::SEVERITY, \
		__LINE__, \
		__FILE__, \
		__FUNCTION__ \
	)

#ifdef LOG_MODULE_NAME
	// LOG_MODULE_NAME is supposed to contain log name
	#define LOG_TRACE    LOG_GENERIC__(LTRACE)(LOG_MODULE_NAME)
	#define LOG_DEBUG    LOG_GENERIC__(LDEBUG)(LOG_MODULE_NAME)
	#define LOG_INFO     LOG_GENERIC__(LINFO)(LOG_MODULE_NAME)
	#define LOG_NOTICE   LOG_GENERIC__(LNOTICE)(LOG_MODULE_NAME)
	#define LOG_ERROR    LOG_GENERIC__(LERROR)(LOG_MODULE_NAME)
	#define LOG_CRITICAL LOG_GENERIC__(LCRITICAL)(LOG_MODULE_NAME)
#else
	// use these macros for logging
	#define LOG_TRACE    LOG_GENERIC__(LTRACE)
	#define LOG_DEBUG    LOG_GENERIC__(LDEBUG)
	#define LOG_INFO     LOG_GENERIC__(LINFO)
	#define LOG_NOTICE   LOG_GENERIC__(LNOTICE)
	#define LOG_ERROR    LOG_GENERIC__(LERROR)
	#define LOG_CRITICAL LOG_GENERIC__(LCRITICAL)
#endif

// for printing variables
#define LOG_VALUE(VAL) #VAL << ": " << (VAL)

namespace TBS {
namespace Log2 {

class TBS_API Log
{
public:
	Log(int severity, int line, const char *file, const char *func);
	~Log();
	
	template <typename T>
	Log& operator<< (const T& item)
	{
		if (write)
		{
			message << item;
		}
		return *this;
	}

	Log& operator() (const char *name);
	Log& operator() (const std::string& name);

private:
	bool write;

	int severity;
	int line;
	std::string file;
	std::string function;
	std::string name;

	std::ostringstream message;
};

} // namespace
} // namespace


#endif // included

