#ifndef INCLUDED_SI_UTILITY_LOG_SETUP
#define INCLUDED_SI_UTILITY_LOG_SETUP

#include <string>

namespace TBS {
namespace Log2 {

void setupSimpleFileLogger(const std::string& filename, int level);
void setupPocoLogger();

} // namespace
} // namespace

#endif // included
