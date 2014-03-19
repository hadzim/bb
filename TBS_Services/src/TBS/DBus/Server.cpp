#ifndef _NO_DBUS
/*
 * Server.cpp
 *
 *  Created on: Sep 29, 2013
 *      Author: root
 */
#include <cstdio>

#include <cstdarg>
#include "dbus-c++/debug.h"

namespace TBS {
		namespace Services {

			void hdbg(const char *format, ...)
			{
			//#ifdef DEBUG


			  {
			    va_list args;
			    va_start(args, format);

			    fprintf(stderr, "dbus-c++: ");
			    vfprintf(stderr, format, args);
			    fprintf(stderr, "\n");

			    va_end(args);
			  }

			//#endif//DEBUG
			}

			bool dbusinit(){
				DBus::debug_log = hdbg;
				return true;
			}
			//bool tmpdbusval = dbusinit();

/*
			class HWIOServer {
				public:
					HWIOServer(HWIO::IHWIO::Ptr impl);
					virtual ~HWIOServer();
				private:

			};
*/
		} /* namespace Services */
} /* namespace TBS */

#endif
