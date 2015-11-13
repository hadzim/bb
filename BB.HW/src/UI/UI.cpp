/*
 * UI.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: dron
 */

#include "UI.h"

#include <opencv2/opencv.hpp>
#include <Poco/Delegate.h>
#include "BB/Screen/NokiaScreen.h"
#include "BB/Screen/OpenCvScreen.h"

#include "Subpages/DatePage.h"

#include "Subpages/LogoPage.h"
#include "Subpages/NetworkPage.h"
#include <BB/ServiceNotification.h>

namespace BB {

	UI::UI() {

#ifdef TBS_DEVICE
		std::cout << "CREATE NOKIA DEVICE" << std::endl;
	screen = new NokiaScreen(50,60, 1, 0, "pwm_test_P9_42.10");
#else
	screen = new OpenCvScreen("bb");
#endif
	Poco::Thread::sleep(100);

		this->t.Timer += Poco::delegate(this, &UI::onTimer);

		this->pages.push_back(new LogoPage(screen));
		this->pages.push_back(new DatePage(screen));
		this->pages.push_back(new NetworkPage(screen));

		for (auto p : pages){
			names.insert(p->getName());
		}

		this->current = this->pages.end();
		showNext();

		t.start(5000,5000);



	}

	std::set <std::string> & UI::getNames(){
		return names;
	}

	void UI::ignore(std::string name){
		Poco::Mutex::ScopedLock l(im);
		ignoredNames.insert(name);
	}
	void UI::unignore(std::string name){
		Poco::Mutex::ScopedLock l(im);
		ignoredNames.erase(name);
	}

	void UI::setIntesity(int intensity){
		screen->setMaxIntensity(intensity);
	}


	UI::~UI() {
		this->t.Timer -= Poco::delegate(this, &UI::onTimer);
	}


	void UI::onTimer(TBS::SimpleTimer::TimerArg & ){
		this->showNext();
	}
	void UI::showNext(){


		Pages::iterator toHide = current;
		Pages::iterator toShow = current;

		{
			Poco::Mutex::ScopedLock l(im);
			int maxCount = pages.size();
			while (maxCount-- > 0){
				if (toShow != pages.end()){
					toShow++;
				}
				if (toShow == pages.end()){
					toShow = pages.begin();
				}

				if (ignoredNames.find((*toShow)->getName()) == ignoredNames.end()){
					break;
				}
			}
		}

		if (toHide != pages.end()){
			(*toHide)->hide();
		}

		screen->clearDisplay();

		if (toShow != pages.end()){
			(*toShow)->show();
		}
		current = toShow;
		screen->display();
	}

} /* namespace BB */
