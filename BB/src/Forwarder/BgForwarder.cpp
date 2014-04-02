/*
 * BgSender.cpp
 *
 *  Created on: Feb 2, 2013
 *      Author: root
 */

#include "BB/Forwarder/BgForwarder.h"
#include <iostream>
namespace BB {

	MesssageNotification::MesssageNotification(const SensorData & m)
			: m(m) {

	}

	const SensorData & MesssageNotification::message() const {
		return this->m;
	}

	BgForwarder::BgForwarder(std::string name, IForwarder::Ptr wrappedForwarder)
			: TBS::NotificationBasedRunnable(name), wrappedForwarder(wrappedForwarder) {

	}

	BgForwarder::~BgForwarder() {

	}

	void BgForwarder::forward(const SensorData & d) {
		this->enqueueNotification(new MesssageNotification(d));
	}

	void BgForwarder::processNotification(Poco::Notification::Ptr notification) {
		//try {
		if (MesssageNotification::Ptr msgNtf = notification.cast<MesssageNotification>()) {
			SensorData d = msgNtf->message();
			wrappedForwarder->forward(d);
		}
		//} catch (Poco::Exception & e) {
		//	std::cerr << "Exc: " << e.displayText() << std::endl;
		//}
	}

} /* namespace TBS */
