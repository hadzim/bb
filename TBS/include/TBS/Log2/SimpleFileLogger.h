#ifndef INCLUDED_UTILITY_SIMPLE_FILE_LOGGER
#define INCLUDED_UTILITY_SIMPLE_FILE_LOGGER

#include <string>
#include <fstream>
#include <vector>
#include "Poco/SharedPtr.h"
#include "LoggerInterface.h"

namespace TBS {
namespace Log2 {

class SimpleFileLogger: public LoggerInterface
{
public:
	typedef Poco::SharedPtr<SimpleFileLogger> SharedPtr;
	struct Setup
	{
		std::string file;
		int severityThreshold;
	};

	SimpleFileLogger(const Setup& setup);
	virtual ~SimpleFileLogger();

	virtual void write(const std::string& name, int severity, int line, const std::string& file, const std::string& func, const std::string& message);
	virtual int getSeverityTreshold(void);
	
	void addToBlacklist(const std::string& name);

private:
	std::vector<std::string> blacklist;
	std::fstream logFile;
	int severityThreshold;
};

} // namespace
} // namespace

#endif // included

