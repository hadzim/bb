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
#include <BB/Sensor/SensorData2.h>
#include <BB/Task/Task.h>
#include "BB/Services/DataSvc_DBus.h"

#include "BB/Sensor/SensorData.h"

namespace BB {

	class IDataSender2 {
		public:
			typedef Poco::SharedPtr <IDataSender2> Ptr;

			virtual ~IDataSender2(){
			}

			virtual void send(const std::string & sensorUID, const Sensor::Data & data) = 0;
			virtual void send(const Sensor::Info & info) = 0;

			static IDataSender2::Ptr create(std::string name, std::string projectID);
			static IDataSender2::Ptr createRemote(std::string name, std::string projectID, std::string serverAddress);
		private:
	};

} /* namespace BB */

#endif /* DATASENDER_H_ */
