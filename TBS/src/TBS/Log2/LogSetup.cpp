#include "TBS/Log2/LogSetup.h"
#include "TBS/Log2/SimpleFileLogger.h"
#include "TBS/Log2/PocoLogger.h"
#include "TBS/Log2/LoggerHolder.h"

namespace TBS {
namespace Log2 {

void setupSimpleFileLogger(const std::string& filename, int level) {
	SimpleFileLogger::Setup loggerSetup;
	loggerSetup.file.assign(filename);
	loggerSetup.severityThreshold = level;
	SimpleFileLogger::SharedPtr logger(new SimpleFileLogger(loggerSetup));
	LoggerHolder::setInstance(logger);
};

void setupPocoLogger() {
	PocoLogger::SharedPtr logger(new PocoLogger());
	LoggerHolder::setInstance(logger);
}

} // namespace
} // namespace

