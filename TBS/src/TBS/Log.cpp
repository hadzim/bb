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

#ifndef _WIN32
#include <unistd.h>
#include <sys/syscall.h>

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

#endif

namespace TBS {

	std::string logBasename(const std::string& fullname){
#ifdef _WIN32
		size_t last_delim = fullname.rfind('\\');
#else
		size_t last_delim = fullname.rfind('/');
#endif
		if (last_delim == std::string::npos) return fullname;
		else return fullname.substr(last_delim+1);
	}

}


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
			Poco::LocalDateTime t;
			std::string lctime = Poco::DateTimeFormatter::format(t, "%M:%S.%i");
#ifndef _WIN32


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
			std::cout << color << lctime << ": " << msg.getText() << colorEnd << std::endl;
#else
			std::cout << lctime << ": " << msg.getText() << std::endl;
#endif

		}

};




namespace TBS {

		void threadDebug() {
			int tid = 0;

			#ifndef _WIN32
						 tid = (pid_t) syscall(SYS_gettid);
			#endif

			Poco::Thread * thread = Poco::Thread::current();
			if (!thread){
				LNOTICE("THRDBG") << "Thread " << "MAIN" << " has Thread ID " << Poco::Thread::currentTid() << " and PID " << tid << LE
			} else {
				LNOTICE("THRDBG") << "Thread " << thread->name() << " has Thread ID " << Poco::Thread::current()->tid() << " and PID " << tid << LE
			}

		}

		void shortFileList(std::set<std::string> files, unsigned int targetListSize) {
					try {
						while (files.size() > targetListSize) {

							std::set<std::string>::iterator it = files.begin();
							Poco::File fToDelete(*it);
							++it;
							for (; it != files.end(); ++it) {

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

				void separeLog(std::string logName, std::string fileName, int level){


						if (Poco::File(fileName).exists()){
							Poco::File(fileName).remove();
						}
						if (Poco::File(fileName + ".0").exists()){
							Poco::File(fileName + ".0").remove();
						}

						Poco::AutoPtr<Poco::SimpleFileChannel> identChannel(new Poco::SimpleFileChannel);

						identChannel->setProperty("path", fileName);
#ifdef __WIN32
						identChannel->setProperty("rotation", "4096 K");
#else
						identChannel->setProperty("rotation", "1024 K");
#endif
						Poco::AutoPtr<Poco::PatternFormatter> identPF(new Poco::PatternFormatter);
						identPF->setProperty("pattern", "%q %H:%M:%S.%i %s<%T>: %t");
						Poco::AutoPtr<Poco::FormattingChannel> identFC(new Poco::FormattingChannel(identPF, identChannel));


						Poco::AutoPtr<Poco::SplitterChannel> pSplitter(new Poco::SplitterChannel());
						pSplitter->addChannel(identFC);
						Poco::AutoPtr<ColorConsoleChannel> pColor(new ColorConsoleChannel());
						pSplitter->addChannel(pColor);

						Poco::Logger::get(logName).setChannel(pSplitter);
						Poco::Logger::get(logName).setLevel((Poco::Message::Priority)level);

						LNOTICE(logName) << "log init" << LE

				}

				void separateIdentLog(std::string name){
					separeLog("IDENT", name);
				}
				void separateBgEnrollmentLog(std::string name){
					separeLog("BGENROLL", name);
				}


				static void initLogsInternally(std::string logName, int level, std::string logDir, Nullable<LogHistory> history = Nullable<LogHistory>(), Poco::Channel * channel = NULL){
					std::cout << "init log " << logName << " to level " << level << std::endl;
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
#ifdef __WIN32
					pCons->setProperty("rotation", "4096 K");
#else
					pCons->setProperty("rotation", "1024 K");
#endif
					Poco::AutoPtr<Poco::PatternFormatter> pPF(new Poco::PatternFormatter);
					pPF->setProperty("pattern", "%q %H:%M:%S.%i %s<%T>: %t");
					Poco::AutoPtr<Poco::FormattingChannel> pFC(new Poco::FormattingChannel(pPF, pCons));

					Poco::AutoPtr<Poco::SplitterChannel> pSplitter(new Poco::SplitterChannel());
					pSplitter->addChannel(pFC);
					Poco::AutoPtr<ColorConsoleChannel> pColor(new ColorConsoleChannel());
					pSplitter->addChannel(pColor);

					if (channel != NULL){
						pSplitter->addChannel(channel);
					}


					Poco::Logger::root().setChannel(pSplitter);
					Poco::Logger::get("TS").setChannel(pSplitter);
					Poco::Logger::get("TRC").setChannel(pSplitter);
					Poco::Logger::root().setLevel((Poco::Message::Priority)level);
					Poco::Logger::get("TS").setLevel((Poco::Message::Priority)level);
					Poco::Logger::get("TRC").setLevel((Poco::Message::Priority)level);


					threadDebug();
				}

				void initLogs(std::string logName, int level, std::string logDir){
					initLogsInternally(logName, level, logDir);
				}
				void initLogs(std::string logName, int level, std::string logDir, LogHistory history, Poco::Channel * channel){
					initLogsInternally(logName, level, logDir, history, channel);
				}

				void initSepareLogs(const std::set<std::string> & names, std::string dir, std::string prefix){
					for (std::set<std::string>::const_iterator slog = names.begin(); slog != names.end(); ++slog){
						std::string slogname = *slog;

						//try parse for @ for distinguish log level
						Poco::StringTokenizer stoken(slogname, "@");
						int level = 8;
						if (stoken.count() == 2 && Poco::NumberParser::tryParse(stoken[1], level)){
							slogname = stoken[0];
							separeLog(slogname, dir + prefix + slogname +".log", level);
						} else {
							separeLog(slogname, dir + prefix + slogname +".log");
						}
					}
				}

				void dumpBacktrace(std::string name, std::string logname, int level){
#ifndef _WIN32

				   Poco::Logger & pl = Poco::Logger::get(logname);

				 //  LERROR(logname) << "dump " << name << "logname" << logname << " level " << level << " loglevel" << pl.getLevel() << LE;

				   if (pl.getLevel() >= level){

					   void *array[10];
					   size_t size;
					   char **strings;
					   size_t i;

					   size = backtrace (array, 10);
					   strings = backtrace_symbols (array, size);

					   Poco::LogStream str(pl);
					   str.priority((Poco::Message::Priority)level);
					   str << name << " - backtrace: " << size << " stack frames (in " << ::TBS::logBasename(__FILE__) << ":" << __func__ << ":" << __LINE__ << ")"<< std::endl;

					   for (i = 0; i < size; i++){
						   str  << " > " << strings[i] << " (in " << ::TBS::logBasename(__FILE__) << ":" << __func__ << ":" << __LINE__ << ")"<< std::endl;
					   }

					   free (strings);
				   }
#endif

				}


} // namespace TBS
