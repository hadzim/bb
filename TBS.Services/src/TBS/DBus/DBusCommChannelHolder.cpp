#ifndef _NO_DBUS

#include "TBS/Services/DBus/DBusCommChannelHolder.h"
#include "TBS/Log.h"
#include <iostream>
/*
 * DBusCommChannelHolder.cpp
 *
 *  Created on: Oct 9, 2013
 *      Author: root
 */



namespace TBS {
	namespace Services {

		DBusCommChannelHolder::DBusCommChannelHolder(std::string name) :
				t("DBus" + name), running(false) {
			dispatcher_ = new ::DBus::BusDispatcher();
			std::cout << "DBusCommChannelHolder C" << std::endl;
		}

		DBusCommChannelHolder::~DBusCommChannelHolder() {
			std::cout << "DBusCommChannelHolder D" << std::endl;
			LTRACE("DBUS") << "DBusCommChannelHolder D" << LE;
			this->stop();
			LTRACE("DBUS") << "DBusCommChannelHolder D after stop" << LE;
			delete dispatcher_;
			LTRACE("DBUS") << "DBusCommChannelHolder D after delete" << LE;
			//std::cout << "dbus destuct after stop" << std::endl;
		}

		void DBusCommChannelHolder::start() {
			//std::cout << "dbus thread started" << std::endl;
			t.start(*this);
			running = true;
		}

		void DBusCommChannelHolder::stop() {
			//std::cout << "dbus thread stop" << std::endl;
			running = false;
			dispatcher_->leave();
			t.join();
			//std::cout << "dbus thread stop done" << std::endl;
		}

		bool DBusCommChannelHolder::isRunning() {
			return running;
		}

		void DBusCommChannelHolder::run() {
			try {
				TBS::threadDebug();
				LTRACE("DBUS") << "dbus thread started BG" << LE;
				dispatcher_->enter();
				LTRACE("DBUS") << "dbus thread finished BG" << LE;
			} catch (::DBus::Error & e){
				LWARNING("DBUS") << "dbus thread BG dbus exc:" << e.message() << LE;
				std::cout << "dbus thread started exception: " << e.message() << std::endl;
			}  catch (Poco::Exception & e){
				LWARNING("DBUS") << "dbus thread BG poco exc:" << e.displayText() << LE;
				std::cout << "dbus thread started exception: " << e.displayText() << std::endl;
			} catch (std::exception & e){
				LWARNING("DBUS") << "dbus thread BG std exc:" << e.what() << LE;
				std::cout << "dbus thread started exception: " << e.what() << std::endl;
			} catch (...){
				LWARNING("DBUS") << "dbus thread BG unknown exc" << LE;
				std::cout << "dbus thread ??? exception " << std::endl;
			}
			LTRACE("DBUS") << "dbus thread done BG" << LE;
		}
		::DBus::BusDispatcher & DBusCommChannelHolder::dispatcher(){
			
			return *this->dispatcher_;
		}

	} /* namespace Services */
} /* namespace TBS */

#endif
