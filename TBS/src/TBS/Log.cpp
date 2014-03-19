/*
 * Log.cpp
 *
 *  Created on: Aug 7, 2012
 *      Author: root
 */

#include "TBS/Log.h"
#include <Poco/Thread.h>


#include <Poco/SimpleFileChannel.h>
#include "Poco/AutoPtr.h"
#include <Poco/PatternFormatter.h>
#include "TBS/Log.h"
#include <Poco/FormattingChannel.h>
#include <iostream>
#include <set>
#include <Poco/Glob.h>

#include <sstream>

#include <Poco/Exception.h>
#include <Poco/StringTokenizer.h>
#include <Poco/String.h>

#include <Poco/File.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeFormat.h>
#include "Poco/Channel.h"

#include <Poco/NumberParser.h>
#include <TBS/Nullable.h>
#include "Poco/NumberFormatter.h"
#include <string>

#ifndef __WIN32__
#include <unistd.h>
#include <sys/syscall.h>

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

#endif



/*
 * EventLogChannel.h
 *
 *  Created on: 13.3.2012
 *      Author: Honza
 */

#include <Poco/Channel.h>
#include <Poco/Message.h>
#include <Poco/SplitterChannel.h>
#include <iostream>
class ColorConsoleChannel : public Poco::Channel {
	public:
		typedef Poco::AutoPtr <ColorConsoleChannel> Ptr;

		ColorConsoleChannel(){}
		virtual ~ColorConsoleChannel(){}

		void log(const Poco::Message& msg){
#ifndef __WIN32__
			Poco::LocalDateTime t;
			std::string lctime = Poco::DateTimeFormatter::format(t, "%M:%S.%i");

			std::string color = "\033[38;5;146m";
			//std::string utctime = Poco::DateTimeFormatter::format(t.utc(), "%Y-%m-%d %H:%M:%S.%i");
			//if (msg.getPriority() >= Poco::Message::PRIO_WARNING){
				//color = "\033[38;5;149m";
				if (msg.getPriority() == Poco::Message::PRIO_NOTICE){
					color = "\033[38;5;74m";
				}
				if (msg.getPriority() == Poco::Message::PRIO_WARNING){
					color = "\033[38;5;148m";
				}
				if (msg.getPriority() == Poco::Message::PRIO_ERROR){
					color = "\033[38;5;196m";
				}
				if (msg.getPriority() <= Poco::Message::PRIO_CRITICAL){
					color = "\033[38;5;200m";
				}
			//}
			std::string colorEnd = "\033[39m";
			std::cout << color << lctime << ": " << msg.getText() << colorEnd << "\n";
#endif

		}

};




namespace TBS {

		void threadDebug() {
			int tid = 0;

			#ifndef __WIN32__
						 tid = (pid_t) syscall(SYS_gettid);
			#endif

			Poco::Thread * thread = Poco::Thread::current();
			if (!thread){
				LOG_NAMED_STREAM_NOTICE("THRDBG") << "Thread " << "MAIN" << " has Thread ID " << Poco::Thread::currentTid() << " and PID " << tid << LOG_STREAM_END
			} else {
				LOG_NAMED_STREAM_NOTICE("THRDBG") << "Thread " << thread->name() << " has Thread ID " << Poco::Thread::current()->tid() << " and PID " << tid << LOG_STREAM_END
			}

		}

		void shortFileList(std::set<std::string> files, unsigned int targetListSize) {
					try {
						while (files.size() > targetListSize) {

							std::set<std::string>::iterator it = files.begin();
							Poco::File fToDelete(*it);
							it++;
							for (; it != files.end(); it++) {

								Poco::File tmpFile(*it);
								if (tmpFile.exists() && tmpFile.created() < fToDelete.created()) {
									fToDelete = tmpFile;
								}
							}
							fToDelete.remove();
							files.erase(fToDelete.path());
						}
					} catch (Poco::Exception & e){
						std::cerr << e.displayText() << std::endl;
					}
				}

				void separeLog(std::string logName, std::string fileName){

						Poco::AutoPtr<Poco::SimpleFileChannel> identChannel(new Poco::SimpleFileChannel);

						identChannel->setProperty("path", fileName);
						identChannel->setProperty("rotation", "1024 K");

						Poco::AutoPtr<Poco::PatternFormatter> identPF(new Poco::PatternFormatter);
						identPF->setProperty("pattern", "%q %H:%M:%S.%i %s<%T>: %t");
						Poco::AutoPtr<Poco::FormattingChannel> identFC(new Poco::FormattingChannel(identPF, identChannel));

						Poco::Logger::get(logName).setChannel(identFC);
						Poco::Logger::get(logName).setLevel(Poco::Message::PRIO_TRACE);

						LOG_NAMED_STREAM_NOTICE(logName) << "log init" << LOG_STREAM_END

				}

				void separateIdentLog(std::string name){
					separeLog("IDENT", name);
				}
				void separateBgEnrollmentLog(std::string name){
					separeLog("BGENROLL", name);
				}


				static void initLogsInternally(std::string logName, int level, std::string logDir, Nullable<LogHistory> history = Nullable<LogHistory>()){
					std::string logFile =  logName + std::string (".log");
					std::string logFile0 = logName + std::string (".log.0");

					Poco::File f(logDir + logFile);
					Poco::File f0(logDir + logFile0);
					if (f.exists()) {
						if (history.isSet()){
							std::set<std::string> logFiles;
							std::set<std::string> logFiles0;
							Poco::Glob::glob(history.ref().path + logFile + "_*", logFiles);
							Poco::Glob::glob(history.ref().path + logFile0 + "_*", logFiles0);
							shortFileList(logFiles, history.ref().size);
							shortFileList(logFiles0,history.ref().size);
							Poco::Timestamp now;
							f.moveTo(history.ref().path + logFile + "_" + Poco::DateTimeFormatter::format(now, Poco::DateTimeFormat::ISO8601_FORMAT));
							if (f0.exists()) {
								f0.moveTo(history.ref().path + logFile0 + "_" + Poco::DateTimeFormatter::format(now, Poco::DateTimeFormat::ISO8601_FORMAT));
							}
						} else {
							f.remove();
							if (f0.exists()) {
								f0.remove();
							}
						}

					}

					if (history.isSet()){
						system(("chmod 0644 " + history.ref().path  + "*.log*").c_str());
						system(("chmod 0644 " + history.ref().path  + "*.log0*").c_str());
					}

					Poco::AutoPtr<Poco::SimpleFileChannel> pCons(new Poco::SimpleFileChannel);

					pCons->setProperty("path", logDir + logFile);
					pCons->setProperty("rotation", "1024 K");

					Poco::AutoPtr<Poco::PatternFormatter> pPF(new Poco::PatternFormatter);
					pPF->setProperty("pattern", "%q %H:%M:%S.%i %s<%T>: %t");
					Poco::AutoPtr<Poco::FormattingChannel> pFC(new Poco::FormattingChannel(pPF, pCons));

					std::cout << "LOGGING IS " << level << std::endl;

					Poco::AutoPtr<Poco::SplitterChannel> pSplitter(new Poco::SplitterChannel());
					pSplitter->addChannel(pFC);
					Poco::AutoPtr<ColorConsoleChannel> pColor(new ColorConsoleChannel());
					pSplitter->addChannel(pColor);


					Poco::Logger::root().setChannel(pSplitter);
					Poco::Logger::get("TS").setChannel(pSplitter);
					Poco::Logger::root().setLevel((Poco::Message::Priority)level);
					Poco::Logger::get("TS").setLevel((Poco::Message::Priority)level);


					threadDebug();
				}

				void initLogs(std::string logName, int level, std::string logDir){
					initLogsInternally(logName, level, logDir);
				}
				void initLogs(std::string logName, int level, std::string logDir, LogHistory history){
					initLogsInternally(logName, level, logDir, history);
				}


				void dumpBacktrace(std::string name, std::string logname, bool isWarning){
#ifndef __WIN32__
				   void *array[10];
				   size_t size;
				   char **strings;
				   size_t i;

				   size = backtrace (array, 10);
				   strings = backtrace_symbols (array, size);
				   if (isWarning){
					   LWARNING(logname) << name << "- backtrace: " << size << " stack frames" << LOG_END;
				   } else {
					   LDEBUG(logname) << name << "- backtrace: " << size << " stack frames" << LOG_END;
				   }
				   for (i = 0; i < size; i++){
					   if (isWarning){
						   LWARNING(logname) << "> " << strings[i] << LOG_END;
					   } else {
						   LDEBUG(logname) << "> " << strings[i] << LOG_END;
					   }
				   }

				   free (strings);
#endif
				}


} // namespace TBS
