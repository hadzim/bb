/*
 * DataSender.h
 *
 *  Created on: Nov 11, 2014
 *      Author: dron
 */

#ifndef DATASENDER_H_
#define DATASENDER_H_
#include <BB/Notification.h>
#include <BB/RuntimeStatus.h>
#include <BB/Task/Task.h>
#include "BB/Services/DataSvc_DBus.h"

#include "BB/Sensor/SensorData.h"

namespace BB {

	class DataSender {
		public:
			DataSender();
			~DataSender();

			void send(const SensorData & task);
			void send(const RuntimeStatus & task);
			void send(const Notification & task);
			void send(const Task & task);
		private:
			TBS::BB::Services::Data::DBus::Client client;
	};

} /* namespace BB */

#endif /* DATASENDER_H_ */
