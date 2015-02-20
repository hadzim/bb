#include "TBS/Log2/LoggerHolder.h"
#include "TBS/Log2/PocoLogger.h"

namespace TBS {
namespace Log2 {

LoggerInterface::SharedPtr LoggerHolder::getInstance(void)
{
	return instance;
}

void LoggerHolder::setInstance(LoggerInterface::SharedPtr newInstance)
{
	instance = newInstance;
}

void LoggerHolder::resetToDefault(void)
{
	instance.assign(new PocoLogger());
}

LoggerInterface::SharedPtr LoggerHolder::instance(new PocoLogger());


} // namespace
} // namespace


