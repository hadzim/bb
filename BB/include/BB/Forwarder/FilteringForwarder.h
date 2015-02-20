/*
 * FilteringForwarder.h
 *
 *  Created on: 2.4.2014
 *      Author: JV
 */

#ifndef FilteringForwarder_H_
#define FilteringForwarder_H_
#include "IForwarder.h"
#include "TBS/Nullable.h"
#include "Filter.h"

namespace BB {


	class FilteringForwarder: public IForwarder {
		public:
			typedef Poco::SharedPtr <FilteringForwarder> Ptr;

			FilteringForwarder(IForwarder::Ptr wrappedForwarder);
			virtual ~FilteringForwarder();

			void addFilter(IFilter::Ptr f);

			virtual void forward(const SensorData & d);
			virtual void forward(const RuntimeStatus & s);
			virtual void forward(const Task & t);
			virtual void forward(const Notification & t);

		private:
			Poco::Mutex m;
			IForwarder::Ptr wrappedForwarder;
			typedef IFilter::PtrList Filters;
			Filters filters;
	};

} /* namespace BB */
#endif /* FilteringForwarder_H_ */
