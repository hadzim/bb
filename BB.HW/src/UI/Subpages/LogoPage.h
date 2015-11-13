/*
 * LogoPage.h
 *
 *  Created on: Apr 8, 2015
 *      Author: dron
 */

#ifndef LOGOPAGE_H_
#define LOGOPAGE_H_

#include "../Page.h"

namespace BB {

	class LogoPage : public Page {
		public:
			LogoPage(IScreen::Ptr screen);
			virtual ~LogoPage();

			virtual void show();

	};

} /* namespace BB */

#endif /* LOGOPAGE_H_ */
