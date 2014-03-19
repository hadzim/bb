#ifndef _NO_DBUS

#include "TBS/Services/DBus/DBusCommChannelHolder.h"
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
				t("DBus" + name) {

		}

		DBusCommChannelHolder::~DBusCommChannelHolder() {
			std::cout << "dbus destuct entered" << std::endl;
			this->stop();
			std::cout << "dbus destuct after stop" << std::endl;
		}

		void DBusCommChannelHolder::start() {
			std::cout << "dbus thread started" << std::endl;
			t.start(*this);
			running = true;
		}

		void DBusCommChannelHolder::stop() {
			std::cout << "dbus thread stop" << std::endl;
			running = false;
			dispatcher_.leave();
			std::cout << "dbus thread stop done" << std::endl;
		}

		bool DBusCommChannelHolder::isRunning() {
			return running;
		}

		void DBusCommChannelHolder::run() {
			try {
				std::cout << "dbus thread started BG" << std::endl;
				dispatcher_.enter();
				std::cout << "dbus thread finished BG" << std::endl;
			} catch (::DBus::Error & e){
				std::cout << "dbus thread started exception: " << e.message() << std::endl;
			}  catch (Poco::Exception & e){
				std::cout << "dbus thread started exception: " << e.displayText() << std::endl;
			} catch (std::exception & e){
				std::cout << "dbus thread started exception: " << e.what() << std::endl;
			}
		}
		::DBus::BusDispatcher & DBusCommChannelHolder::dispatcher(){
			std::cout << "get dispatcher" << std::endl;

			return this->dispatcher_;
		}

	} /* namespace Services */
} /* namespace TBS */

#endif
