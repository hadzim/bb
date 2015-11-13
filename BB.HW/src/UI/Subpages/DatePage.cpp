/*
 * DatePage.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: dron
 */

#include "DatePage.h"
#include <string>
#include <iostream>

#include <Poco/DateTime.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/LocalDateTime.h>
#include <sstream>
#include <ios>
#include <iomanip>

namespace BB {

	DatePage::DatePage(IScreen::Ptr screen) :
			Page("date", screen) {

	}

	DatePage::~DatePage() {

	}

	void DatePage::show() {
		std::cout << "show ip" << std::endl;
		screen->setTextColor(1);
		screen->setTextSize(1);

		screen->setCursor(30, 2);
		screen->print("Date");

		Poco::LocalDateTime localTime;

		screen->setCursor(3, 36);
		{
			std::stringstream s;
			s << localTime.day() << "." << localTime.month() << ".";
			screen->print(s.str());
		}

		screen->setCursor(64, 36);
		{

			screen->print( Poco::DateTimeFormatter::format(localTime, "%w"));
		}

		screen->setCursor(10, 16);
		screen->setTextSize(2);
		{
			screen->print( Poco::DateTimeFormatter::format(localTime, "%H:%M"));
		}

	}

} /* namespace BB */
