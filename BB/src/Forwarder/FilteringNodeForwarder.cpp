/*
 * FilteringForwarder.cpp
 *
 *  Created on: 2.4.2014
 *      Author: JV
 */

#include "BB/Forwarder/FilteringForwarder.h"

#include "TBS/Log.h"

namespace BB {

FilteringForwarder::FilteringForwarder(IForwarder::Ptr wrappedForwarder) : wrappedForwarder(wrappedForwarder) {

}

FilteringForwarder::~FilteringForwarder() {

}

void FilteringForwarder::addFilter(IFilter::Ptr f){
	Poco::Mutex::ScopedLock l(m);
	this->filters.push_back(f);
}

void FilteringForwarder::forward(const SensorData & d){
	TBS::Nullable <SensorData> result;
	{
		Poco::Mutex::ScopedLock l(m);
		for (Filters::iterator i = filters.begin(); i != filters.end(); i++){
			TBS::Nullable <SensorData> res = (*i)->filter(d);
			if (res.isSet()){
				result = res;
				break;
			}
		}
	}
	if (result.isSet()){
		//std::cout << "filter data OK" << std::endl;
		wrappedForwarder->forward(result.ref());
	} else {
		//std::cout << "filter data REJECTED" << std::endl;
	}
}

void FilteringForwarder::forward(const RuntimeStatus & s){
	LINFO("RC") << "forward status" << LE;
	wrappedForwarder->forward(s);
}
void FilteringForwarder::forward(const Task & t){
	LINFO("RC") << "forward task" << LE;
	wrappedForwarder->forward(t);
}

void FilteringForwarder::forward(const Notification & t){
	LINFO("RC") << "forward notification" << LE;
	wrappedForwarder->forward(t);
}

} /* namespace BB */
