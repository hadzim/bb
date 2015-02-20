/*
 * CommandNotification.h
 *
 *  Created on: Jan 5, 2012
 *      Author: root
 */

#ifndef COMMANDNOTIFICATION_H_
#define COMMANDNOTIFICATION_H_

#include <Poco/Notification.h>

#include "Poco/Event.h"
#include "INotificationWorker.h"
#include "Command.h"

namespace TBS {

	class TBS_API CommandNotification: public Poco::Notification {

		public:
			typedef Poco::AutoPtr<CommandNotification> Ptr;

			CommandNotification(ICommand::Ptr command);
			virtual ~CommandNotification();

			void run();
			virtual std::string name() const;

			Poco::Event CommandDoneEvent;

			bool failed();
			bool passed();
			std::string & errorMessage();
		private:
			ICommand::Ptr command;
			bool done;
			std::string errorMsg;

	};

	TBS_API void __tbsCmdExec(INotificationWorker & nw, ICommand::Ptr command, bool alwaysEnque, bool wait);

	template<bool AlwaysEnque, bool Wait>
	class CommandExecutor {
		public:
			static void execute(INotificationWorker & nw, ICommand::Ptr command) {
				__tbsCmdExec(nw, command, AlwaysEnque, Wait);
			}
			static void execute(INotificationWorker::Ptr nw, ICommand::Ptr command) {
				execute(*(nw.get()), command);
			}
	};

	//typedef CommandExecutor <true, true> CommandExecutorEnqueueAndBlock;
	typedef CommandExecutor<true, false> CommandExecutorEnqueueAndContinue;
	typedef CommandExecutor<false, true> CommandExecutorAutoAndBlock;
	typedef CommandExecutor<false, false> CommandExecutorAutoAndContinue;

} /* namespace TBS */
#endif /* MODULECOMMANDNOTIFICATION_H_ */
