/*
 * ServiceNotification.h
 *
 *  Created on: Jul 9, 2012
 *      Author: root
 */

#ifndef ServiceNotification_H_
#define ServiceNotification_H_

#include <string>
namespace BB {

		class ServiceNotification {
			public:
				static void disable();

				static void enable();

				static void pingWatchdog();

				static void serviceReady(std::string message = "Ready");
				static void serviceDisabled(std::string message = "Disabled");

				static void infoTerminating();
				static void infoFinished();
				static void infoException();
				static void info(std::string);
		};


}

#endif /* WATCHDOG_H_ */
