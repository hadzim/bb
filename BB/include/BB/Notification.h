/*
 * Ntf.h
 *
 *  Created on: Oct 2, 2014
 *      Author: dron
 */

#ifndef BBNOTIFICATION_H_
#define BBNOTIFICATION_H_

#include "TBS/ReadableEnum.h"
#include <iosfwd>
#include <map>
#include <string>

#include "Poco/DateTime.h"

namespace BB {

	class Notification {
		public:


			enum Level {
				Info = 1,
				Warning = 2,
				Alert = 3,
			};

			struct NamesLevel: public TBS::ReadableEnum {
					NamesLevel();
			};


			static NamesLevel namesLevel;


			Notification(Level level, std::string source, std::string message);
			Notification(Level level, std::string source, std::string message, Poco::DateTime date);

			Level getLevel() const;
			std::string getSource() const;
			std::string getMessage() const;
			Poco::DateTime getDate() const;
			std::string getDateAsString() const;

			static std::string readableLevel(Level level);

		private:
			Level level;
			std::string source;
			std::string message;
			Poco::DateTime date;


	};

} /* namespace BB */

std::ostream & operator <<(std::ostream & o, const BB::Notification & notification);

#endif /* RUNTIMESTATUS_H_ */
