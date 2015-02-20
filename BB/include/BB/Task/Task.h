/*
 * Task.h
 *
 *  Created on: Oct 2, 2014
 *      Author: dron
 */

#ifndef BBTASK_H_
#define BBTASK_H_

#include "TBS/ReadableEnum.h"
#include <iosfwd>
#include <map>
#include <string>

#include "Poco/DateTime.h"

#include "json/value.h"

namespace BB {

	class Task {
		public:

			typedef std::map <int, Task> Map;

			static const std::string UpdateStatus;
			static const std::string CaptureCamera;

			Task(std::string what, std::string params = "null", std::string from = "BB", std::string to = "BB");

			std::string getSource() const;	//sender
			std::string getDestination() const;		//receiver

			std::string getWhat() const;
			std::string getParams() const; //json

			int getIntParam(std::string paramName) const;
			std::string getStringParam(std::string paramName) const;

//			Poco::DateTime getDate() const;
//			std::string getDateAsString() const;


		private:
			//StatusMap status;
			//Poco::DateTime date;

			std::string what;
			std::string params; //json

			std::string source;	//sender
			std::string destination;		//receiver

			Json::Value paramsJson;
	};

} /* namespace BB */

std::ostream & operator <<(std::ostream & o, const BB::Task & task);

#endif /* RUNTIMESTATUS_H_ */
