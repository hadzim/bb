/*
 * BgSender.cpp
 *
 *  Created on: Feb 2, 2013
 *      Author: root
 */

#include "BB/Forwarder/BgForwarder.h"
#include <iostream>

#include "TBS/Log.h"
namespace BB {

	MesssageNotification::MesssageNotification(const SensorData & m)
			: m(m) {

	}

	const SensorData & MesssageNotification::message() const {
		return this->m;
	}

	StatusNotification::StatusNotification(const RuntimeStatus & s)
			: s(s) {
	}

	const RuntimeStatus & StatusNotification::status() const {
		return this->s;
	}


	TaskNotification::TaskNotification(const Task & t)
			: t(t) {
	}

	const Task & TaskNotification::task() const {
		return this->t;
	}

	NtfNotification::NtfNotification(const BB::Notification & n) : n(n){}
	const BB::Notification & NtfNotification::ntf() const {
		return n;
	}





	BgForwarder::BgForwarder(std::string name, IForwarder::Ptr wrappedForwarder)
			: TBS::NotificationBasedRunnable(name), wrappedForwarder(wrappedForwarder) {

	}

	BgForwarder::~BgForwarder() {

	}

	void BgForwarder::forward(const SensorData & d) {
		this->enqueueNotification(new MesssageNotification(d));
	}

	void BgForwarder::forward(const RuntimeStatus & d) {
		LDEBUG("RC") << "forward status" << LE;
		this->enqueueNotification(new StatusNotification(d));
	}

	void BgForwarder::forward(const Task & d) {
		LDEBUG("RC") << "forward task" << LE;
		this->enqueueNotification(new TaskNotification(d));
	}

	void BgForwarder::forward(const Notification & n){
		LDEBUG("RC") << "forward ntf" << LE;
		this->enqueueNotification(new NtfNotification(n));
	}

	void BgForwarder::processNotification(Poco::Notification::Ptr notification) {
		//try {

		if (MesssageNotification::Ptr msgNtf = notification.cast<MesssageNotification>()) {
			SensorData d = msgNtf->message();
			wrappedForwarder->forward(d);
		}

		if (StatusNotification::Ptr sNtf = notification.cast<StatusNotification>()) {
			RuntimeStatus s = sNtf->status();
			wrappedForwarder->forward(s);
		}

		if (TaskNotification::Ptr tNtf = notification.cast<TaskNotification>()) {
			Task t = tNtf->task();
			wrappedForwarder->forward(t);
		}

		if (NtfNotification::Ptr tNtf = notification.cast<NtfNotification>()) {
			Notification n = tNtf->ntf();
			wrappedForwarder->forward(n);
		}

		//} catch (Poco::Exception & e) {
		//	std::cerr << "Exc: " << e.displayText() << std::endl;
		//}
	}

} /* namespace TBS */
