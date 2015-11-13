/*
 * UI.h
 *
 *  Created on: Apr 8, 2015
 *      Author: dron
 */

#ifndef UI_H_
#define UI_H_
#include <Poco/SharedPtr.h>
#include "Poco/Util/ServerApplication.h"
#include <TBS/SimpleTimer.h>

#include "BB/Screen/IScreen.h"

#include "Page.h"
#include <set>

#include "Poco/Mutex.h"

namespace BB {

	class UI  {
		public:
			typedef Poco::SharedPtr <UI> Ptr;

			UI();
			virtual ~UI();

			std::set <std::string> & getNames();
			void ignore(std::string);
			void unignore(std::string);

			void setIntesity(int intensity);
		private:

			void onTimer(TBS::SimpleTimer::TimerArg & a);
			void showNext();


		private:
			Poco::Mutex im;
			IScreen::Ptr screen;
			typedef std::vector <Page::Ptr> Pages;
			std::set <std::string> names;
			std::set <std::string> ignoredNames;
			Pages pages;
			Pages::iterator current;

			TBS::SimpleTimer t;
	};

} /* namespace BB */

#endif /* UI_H_ */
