/*
 * Log.h
 *
 *  Created on: 12.10.2011
 *      Author: Honza
 */


#ifndef TBS_LOG_H_
#define TBS_LOG_H_

#include "TBS/TBS.h"

#include <Poco/Logger.h>
#include <Poco/LogStream.h>
#include <Poco/NullStream.h>

#define LOG_THREAD  "THREAD"
#define LOG_TIME 	"TIME"

#ifdef NO_LOGGING
	#define LOG_STREAM { if (1){} else { Poco::NullOutputStream str; str
	#define LOG_STREAM_END std::endl; }}

	#define LOG_STREAM_FATAL LOG_STREAM
	#define LOG_STREAM_CRITICAL LOG_STREAM
	#define LOG_STREAM_ERROR LOG_STREAM
	#define LOG_STREAM_WARNING LOG_STREAM
	#define LOG_STREAM_NOTICE LOG_STREAM
    #define LOG_STREAM_INFO LOG_STREAM
	#define LOG_STREAM_DEBUG LOG_STREAM
    #define LOG_STREAM_TRACE LOG_STREAM

	#define LOG_NAMED_STREAM_FATAL(name) LOG_STREAM
	#define LOG_NAMED_STREAM_CRITICAL(name) LOG_STREAM
	#define LOG_NAMED_STREAM_ERROR(name) LOG_STREAM
	#define LOG_NAMED_STREAM_WARNING(name)  LOG_STREAM
	#define LOG_NAMED_STREAM_NOTICE(name)  LOG_STREAM
    #define LOG_NAMED_STREAM_INFO(name) LOG_STREAM
	#define LOG_NAMED_STREAM_DEBUG(name) LOG_STREAM
    #define LOG_NAMED_STREAM_TRACE(name) LOG_STREAM

#else


	#define LOG_STREAM { Poco::LogStream(Poco::Logger::get(__FILE__)) str; { str
	#define LOG_STREAM_END " (in " << __FILE__ << ":" << __func__ << ":" << __LINE__ << ")"<< std::endl; }}
	#define LE LOG_STREAM_END

	#define LFATAL(name) { Poco::Logger & pl = Poco::Logger::get(name); Poco::LogStream str(pl); if (pl.fatal()) { str.fatal()
	#define LCRITICAL(name) { Poco::Logger & pl = Poco::Logger::get(name); Poco::LogStream str(pl); if (pl.critical()) { str.critical()
	#define LERROR(name) { Poco::Logger & pl = Poco::Logger::get(name); Poco::LogStream str(pl); if (pl.error()) { str.error()
	#define LWARNING(name)  { Poco::Logger & pl = Poco::Logger::get(name); Poco::LogStream str(pl); if (pl.warning()) { str.warning()
	#define LNOTICE(name)  { Poco::Logger & pl = Poco::Logger::get(name); Poco::LogStream str(pl); if (pl.notice()) { str.notice()
    #define LINFO(name) { Poco::Logger & pl = Poco::Logger::get(name); Poco::LogStream str(pl); if (pl.information()) { str.information()
	#define LDEBUG(name) { Poco::Logger & pl = Poco::Logger::get(name); Poco::LogStream str(pl); if (pl.debug()) { str.debug()
    #define LTRACE(name) { Poco::Logger & pl = Poco::Logger::get(name); Poco::LogStream str(pl); if (pl.trace()) { str.trace()


	#define LG_FATAL(name) { Poco::Logger & pl = Poco::Logger::get(name); Poco::LogStream str(pl); if (pl.fatal()) { str.fatal()
	#define LG_CRITICAL(name) { Poco::Logger & pl = Poco::Logger::get(name); Poco::LogStream str(pl); if (pl.critical()) { str.critical()
	#define LG_ERROR(name) { Poco::Logger & pl = Poco::Logger::get(name); Poco::LogStream str(pl); if (pl.error()) { str.error()
	#define LG_WARNING(name)  { Poco::Logger & pl = Poco::Logger::get(name); Poco::LogStream str(pl); if (pl.warning()) { str.warning()
	#define LG_NOTICE(name)  { Poco::Logger & pl = Poco::Logger::get(name); Poco::LogStream str(pl); if (pl.notice()) { str.notice()
    #define LG_INFO(name) { Poco::Logger & pl = Poco::Logger::get(name); Poco::LogStream str(pl); if (pl.information()) { str.information()
	#define LG_DEBUG(name) { Poco::Logger & pl = Poco::Logger::get(name); Poco::LogStream str(pl); if (pl.debug()) { str.debug()
    #define LG_TRACE(name) { Poco::Logger & pl = Poco::Logger::get(name); Poco::LogStream str(pl); if (pl.trace()) { str.trace()

	#define LOG_NAMED_STREAM_FATAL(name) { Poco::Logger & pl = Poco::Logger::get(name); Poco::LogStream str(pl); if (pl.fatal()) { str.fatal()
	#define LOG_NAMED_STREAM_CRITICAL(name) { Poco::Logger & pl = Poco::Logger::get(name); Poco::LogStream str(pl); if (pl.critical()) { str.critical()
	#define LOG_NAMED_STREAM_ERROR(name) { Poco::Logger & pl = Poco::Logger::get(name); Poco::LogStream str(pl); if (pl.error()) { str.error()
	#define LOG_NAMED_STREAM_WARNING(name)  { Poco::Logger & pl = Poco::Logger::get(name); Poco::LogStream str(pl); if (pl.warning()) { str.warning()
	#define LOG_NAMED_STREAM_NOTICE(name)  { Poco::Logger & pl = Poco::Logger::get(name); Poco::LogStream str(pl); if (pl.notice()) { str.notice()
    #define LOG_NAMED_STREAM_INFO(name) { Poco::Logger & pl = Poco::Logger::get(name); Poco::LogStream str(pl); if (pl.information()) { str.information()
	#define LOG_NAMED_STREAM_DEBUG(name) { Poco::Logger & pl = Poco::Logger::get(name); Poco::LogStream str(pl); if (pl.debug()) { str.debug()
    #define LOG_NAMED_STREAM_TRACE(name) { Poco::Logger & pl = Poco::Logger::get(name); Poco::LogStream str(pl); if (pl.trace()) { str.trace()

	#define LOG_STREAM_FATAL LOG_NAMED_STREAM_FATAL("TS")
	#define LOG_STREAM_CRITICAL LOG_NAMED_STREAM_CRITICAL("TS")
	#define LOG_STREAM_ERROR LOG_NAMED_STREAM_ERROR("TS")
	#define LOG_STREAM_WARNING LOG_NAMED_STREAM_WARNING("TS")
	#define LOG_STREAM_NOTICE LOG_NAMED_STREAM_NOTICE("TS")
	#define LOG_STREAM_INFO LOG_NAMED_STREAM_INFO("TS")
	#define LOG_STREAM_DEBUG LOG_NAMED_STREAM_DEBUG("TS")
	#define LOG_STREAM_TRACE LOG_NAMED_STREAM_TRACE("TS")

	#define LOG_STREAM_IDENT LOG_NAMED_STREAM_NOTICE("IDENT")
	#define LOG_STREAM_BGENROLL LOG_NAMED_STREAM_NOTICE("BGENROLL")

	//journal log
	#define LOG_EVENT_ERROR(module) 	LOG_NAMED_STREAM_ERROR   ("TBS." module)
	#define LOG_EVENT_WARNING(module) LOG_NAMED_STREAM_WARNING ("TBS." module)
	#define LOG_EVENT_INFO(module) 	LOG_NAMED_STREAM_INFO    ("TBS." module)
	#define LOG_EVENT_DETAIL " || "
	#define LOG_END std::endl; }}

#endif

#define LOG_INCOMPLETE_IMPLEMENTATION(Of) Poco::Logger::get("Incomplete").warning(Of);
namespace TBS {

		TBS_API void threadDebug();

		struct LogHistory {
				std::string path;
				int size;
		};

		TBS_API void initLogs(std::string logName, int level, std::string logDir = "");
		TBS_API void initLogs(std::string logName, int level, std::string logDir, LogHistory history);

		TBS_API void separeLog(std::string logName, std::string fileName);
		TBS_API void separateIdentLog(std::string logName = "ident.log");
		TBS_API void separateBgEnrollmentLog(std::string logName = "bgEnrollment.log");



		TBS_API void dumpBacktrace(std::string name, std::string logname = "TS", bool isWarning = true);

}


#endif /* LOG_H_ */
