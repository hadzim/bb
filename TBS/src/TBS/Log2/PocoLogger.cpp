#include "TBS/TBS.h"
#include "TBS/Log2/PocoLogger.h"
#include <Poco/Logger.h>
#include <Poco/LogStream.h>
#include <iostream>
#include <sstream>


// To stay compatible with original logger without including it 
namespace TBS { TBS_API std::string logBasename(const std::string& fullname); }

namespace TBS {
namespace Log2 {

PocoLogger::PocoLogger()
{
}

PocoLogger::~PocoLogger()
{
}

void PocoLogger::write(const std::string& name, int severity, int line, const std::string& file, const std::string& func, const std::string& message)
{
	std::string name_nonempty(name);
	if (name_nonempty.empty()) name_nonempty = "Log2";

	//TODO: return back to string when boost::lexical_cast is available
	std::ostringstream result;
	result << message;
	result << " (in " << logBasename(file) << " : " << func << " : " << line << " )";
	Poco::Logger& pl(Poco::Logger::get(name_nonempty));
	Poco::LogStream str(pl);
	switch (severity) {
		case LCRITICAL: if (pl.critical()) str.critical() << result.str() << std::endl; break;
		case LERROR: if (pl.error()) str.error() << result.str() << std::endl; break;
		case LNOTICE: if (pl.notice()) str.notice() << result.str() << std::endl; break;
		case LINFO: if (pl.information()) str.information() << result.str() << std::endl; break;
		case LDEBUG: if (pl.debug()) str.debug() << result.str() << std::endl; break;
		case LTRACE: if (pl.trace()) str.trace() << result.str() << std::endl; break;
	}
}

int PocoLogger::getSeverityTreshold(void)
{
	return 100; // all logs will be assembled, let Poco decide if to write them
	//TODO: optimize, decide first if to assemble the log
}

} // namespace
} // namespace

