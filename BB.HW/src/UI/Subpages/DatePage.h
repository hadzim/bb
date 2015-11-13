/*
 * NetworkPage.h
 *
 *  Created on: Apr 8, 2015
 *      Author: dron
 */

#ifndef DATEPAGE_H_
#define DATEPAGE_H_
#include "../Page.h"

namespace BB {

	class DatePage : public Page {
			public:
			DatePage(IScreen::Ptr screen);
			virtual ~DatePage();

			virtual void show();

		};
} /* namespace BB */

#endif /* NETWORKPAGE_H_ */
