/*
 * Command.h
 *
 *  Created on: Jan 5, 2012
 *      Author: root
 */

#ifndef MODULECOMMAND_H_
#define MODULECOMMAND_H_
#include <Poco/SharedPtr.h>
#include <iostream>
#include "TBS/Log.h"
#include "TBS.h"

namespace TBS {

	class TBS_API ICommand {
		public:

			typedef Poco::SharedPtr<ICommand> Ptr;

			ICommand(std::string name) :
					name(name) {

			}
			virtual ~ICommand() {
			}
			virtual void run() = 0;
			virtual std::string getName() const {
				return name;
			}
		private:
			std::string name;
	};

	template<class OwnerType>
	class Command: public ICommand {
		public:
			typedef void (OwnerType::*Callback)();

			Command(std::string name, OwnerType * pOwner, Callback cmethod) :
					ICommand(name), pOwner(pOwner), cmethod(cmethod) {

			}
			virtual ~Command() {

			}

			virtual void run() {
				LOG_STREAM_INFO<< "run cmd: " << this->getName() << LE;
					(pOwner->*cmethod)();
				}

				private:
				OwnerType * pOwner;
				Callback cmethod;
			};

	template<class OwnerType, class ArgType>
	class CommandArgs: public ICommand {
		public:
			typedef void (OwnerType::*Callback)(ArgType);

			CommandArgs(std::string name, OwnerType * pOwner, Callback cmethod, ArgType arg) :
					ICommand(name), pOwner(pOwner), cmethod(cmethod), arg(arg) {

			}

			virtual ~CommandArgs() {

			}

			virtual void run() {
				LINFO("CMD")<<"run cmd: " << this->getName() << LE;
				(pOwner->*cmethod)(arg);
			}

			private:
			OwnerType * pOwner;
			Callback cmethod;
			ArgType arg;
		};

	}
	/* namespace TBS */
#endif /* MODULECOMMAND_H_ */
