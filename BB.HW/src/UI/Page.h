/*
 * Page.h
 *
 *  Created on: Apr 8, 2015
 *      Author: dron
 */

#ifndef PAGE_H_
#define PAGE_H_
#include <Poco/SharedPtr.h>

#include "BB/Screen/IScreen.h"

namespace BB {

	class Page {
		public:
			typedef Poco::SharedPtr <Page> Ptr;

			Page(std::string name, IScreen::Ptr screen);
			virtual ~Page();

			virtual void show();
			virtual void hide();

			std::string getName();
		protected:
			std::string name;
			IScreen::Ptr screen;

	};

} /* namespace BB */

#endif /* PAGE_H_ */
