
#include "TBS/Log2/SimpleFileLogger.h"
#include <Poco/DateTime.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>

namespace TBS {
namespace Log2 {

SimpleFileLogger::SimpleFileLogger(const Setup& setup):
	severityThreshold(setup.severityThreshold)
{
	logFile.open(setup.file.c_str(), std::ios_base::out|std::ios_base::app);
}

SimpleFileLogger::~SimpleFileLogger()
{
	logFile.close();
}

void SimpleFileLogger::write(const std::string& name, int severity, int line, const std::string& file, const std::string& function, const std::string& message)
{
	if (std::find(blacklist.begin(), blacklist.end(), name) != blacklist.end()) return;

	std::string::size_type shortBegin(file.rfind('/'));
	if (shortBegin == std::string::npos) shortBegin = 0; // take whole file if no slash found
	else ++shortBegin; // skip the slash

	char severityChar('-');
	switch (severity)
	{
		case LCRITICAL: severityChar = 'C'; break;
		case LERROR: severityChar = 'E'; break;
		case LNOTICE: severityChar = 'N'; break;
		case LINFO: severityChar = 'I'; break;
		case LDEBUG: severityChar = 'D'; break;
		case LTRACE: severityChar = 'T'; break;
	}

	Poco::Timestamp time(Poco::LocalDateTime().timestamp());
	std::string time_str(Poco::DateTimeFormatter::format(time, Poco::DateTimeFormat::SORTABLE_FORMAT));

	logFile 
		<< severityChar 
		<< " " << time_str 
		<< " " << file.c_str()+shortBegin 
		<< ":" << line 
		<< " " << function 
		<< ": " << message 
		<< std::endl;
}

int SimpleFileLogger::getSeverityTreshold(void)
{
	return severityThreshold;
}

void SimpleFileLogger::addToBlacklist(const std::string& name)
{
	blacklist.push_back(name);
}

} // namespace
} // namespace


