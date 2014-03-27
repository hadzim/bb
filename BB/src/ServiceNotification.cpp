/*
 * Watchdog.cpp
 *
 *  Created on: Jul 9, 2012
 *      Author: root
 */


#include <BB/ServiceNotification.h>

#include "TBS/Log.h"
#include <sstream>

#if defined(TBS_DEVICE)

#include "unistd.h"
#include <systemd/sd-daemon.h>
#include <stdlib.h>

#endif

namespace BB {


			void ServiceNotification::disable() {
#if defined(TBS_DEVICE)
				system("/home/tbs/scripts/app_watchdog_change.sh disable");
#endif
			}

			void ServiceNotification::enable() {
#if defined(TBS_DEVICE)
				system("/home/tbs/scripts/app_watchdog_change.sh enable");
#endif
			}

			void ServiceNotification::serviceReady(std::string message){
#if defined(TBS_DEVICE)
				LOG_STREAM_DEBUG << "ServiceNotification::serviceReady: " << message << LE;
				std::string msg = "READY=1\n"
						"STATUS=" + message + "\n"
						"MAINPID=%lu";
				sd_notifyf(0,msg.c_str(), (unsigned long) getpid());
#endif
			}
			void ServiceNotification::serviceDisabled(std::string message){
#if defined(TBS_DEVICE)
				LOG_STREAM_DEBUG << "ServiceNotification::serviceDisabled: " << message << LE;
				std::string msg = "READY=0\n"
								"STATUS=" + message + "\n"
								"MAINPID=%lu";
				sd_notifyf(0,msg.c_str(), (unsigned long) getpid());
#endif
			}
			void ServiceNotification::pingWatchdog() {
#if defined(TBS_DEVICE)
				LOG_STREAM_DEBUG << "Watchdog::pingWatchdog()" << LE;
				sd_notify(0, "WATCHDOG=1");
#endif
			}
			void ServiceNotification::infoFinished() {
#if defined(TBS_DEVICE)
				sd_notify(0, "STATUS=Finished");
#endif
			}

			void ServiceNotification::infoTerminating() {
#if defined(TBS_DEVICE)
				sd_notify(0, "STATUS=Terminating");
#endif
			}
			void ServiceNotification::infoException() {
#if defined(TBS_DEVICE)
				sd_notify(0, "STATUS=Exception");
#endif
			}

			void ServiceNotification::info(std::string info) {
#if defined(TBS_DEVICE)
				std::string s = "STATUS=" + info;
				sd_notify(0, s.c_str());
#endif
			}



	}

