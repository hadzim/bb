/*
 * NetworkPage.h
 *
 *  Created on: Apr 8, 2015
 *      Author: dron
 */

#ifndef NETWORKPAGE_H_
#define NETWORKPAGE_H_
#include "../Page.h"

namespace BB {

	class NetworkPage : public Page {
			public:
			NetworkPage(IScreen::Ptr screen);
			virtual ~NetworkPage();

			virtual void show();

		};
} /* namespace BB */

#endif /* NETWORKPAGE_H_ */
