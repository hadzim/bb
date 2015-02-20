/*
 * IForwarder.h
 *
 *  Created on: Feb 1, 2013
 *      Author: root
 */

#ifndef IForwarder_H_
#define IForwarder_H_
#include <BB/Notification.h>
#include <BB/RuntimeStatus.h>
#include <BB/Task/Task.h>
#include "BB/Sensor/SensorData.h"
#include <Poco/SharedPtr.h>

namespace BB {
			class IForwarder {
				public:
					typedef Poco::SharedPtr <IForwarder> Ptr;

					virtual ~IForwarder();

					virtual void forward(const SensorData & d) = 0;
					virtual void forward(const RuntimeStatus & s) = 0;
					virtual void forward(const Task & t) = 0;
					virtual void forward(const Notification & t) = 0;
			};


} /* namespace TBS */
#endif /* IForwarder_H_ */
