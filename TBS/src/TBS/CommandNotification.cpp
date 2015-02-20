/*
 * CommandNotification.cpp
 *
 *  Created on: Jan 5, 2012
 *      Author: root
 */

#include "TBS/CommandNotification.h"
#include <Poco/LogStream.h>
#include <iostream>

namespace TBS {

		CommandNotification::CommandNotification(ICommand::Ptr command) : command(command), done(false){

		}
		CommandNotification::~CommandNotification(){

		}

		void CommandNotification::run(){
			Poco::LogStream stream("ModuleCmd");
			try {
				command->run();
				done = true;
				LINFO("CMD") << "CMD " << command->getName() << " passed" << LE
			} catch (Poco::Exception & e){
				done = false;
				this->errorMsg = "PocoError " + e.displayText();
				LWARNING("CMD") << "CMD" << command->getName() << " failed " << e.displayText() << LE
			} catch (std::exception & e){
				done = false;
				this->errorMsg = "std error " + (std::string)e.what();
				LWARNING("CMD") << "CMD" << command->getName() << " failed " << e.what() << LE
			} catch (...){
				this->errorMsg = "unknown error";
				LWARNING("CMD") << "CMD" << command->getName() << " failed - unknown" << LE
				done = false;
			}
			CommandDoneEvent.set();
		}
		std::string CommandNotification::name() const {
			return (std::string)"CMD::" + command->getName();
		}

		bool CommandNotification::failed(){
			return !done;
		}
		bool CommandNotification::passed(){
			return done;
		}
		std::string & CommandNotification::errorMessage(){
			return this->errorMsg;
		}

		void __tbsCmdExec(INotificationWorker & nw, ICommand::Ptr command, bool alwaysEnque, bool wait){
			if (!alwaysEnque && nw.isBgThread()) {
				command->run();
			} else {
				CommandNotification::Ptr nf = new CommandNotification(command);
				LDEBUG("CMD") << "ENQUE CMD" << command->getName() << LE
				nw.postNotification(nf);
				LDEBUG("CMD") << "WAIT FOR CMD" << command->getName() << LE
				if (wait) {
					try {
						nf->CommandDoneEvent.wait(15000);
						if (nf->failed()) {
							std::string msg = "Command failed: ";
							msg.append(nf->errorMessage());
							throw Poco::Exception(msg);
						}
					} catch (Poco::Exception & e) {
						LERROR("CMD") << "ERROR runCmd " << command->getName() << " FAILED:" << e.displayText() << LE
						throw;
					}
				}
			}
		}

} /* namespace TBS */
