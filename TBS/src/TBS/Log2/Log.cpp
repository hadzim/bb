
#include "TBS/Log2/Log.h"
#include "TBS/Log2/LoggerHolder.h"

namespace TBS {
namespace Log2 {

Log::Log(int severity, int line, const char *file, const char *func)
{
	write = LoggerHolder::getInstance()->getSeverityTreshold() >= severity;

	if (write)
	{
		this->severity = severity;
		this->line = line;
		this->file.assign(file);
		this->function.assign(func);
	}
}

Log& Log::operator() (const char *name)
{
	this->name = name;
	return *this;
}

Log& Log::operator() (const std::string& name)
{
	this->name = name;
	return *this;
}

Log::~Log()
{
	if (write) LoggerHolder::getInstance()->write(name, severity, line, file, function, message.str());
}
	
} // namespace
} // namespace

