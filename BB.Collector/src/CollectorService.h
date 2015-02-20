/*
 * CollectorService.h
 *
 *  Created on: 18.3.2014
 *      Author: JV
 */

#ifndef COLLECTORSERVICE_H_
#define COLLECTORSERVICE_H_

#include "Poco/Util/ServerApplication.h"
#include "BB/Configuration.h"
#include "BB/Services/Data.h"


#include "BB/Sensor/SensorData.h"
#include "BB/Sensor/SensorDataHelpers.h"

#include "BB/RuntimeStatus.h"
#include "BB/Task/Task.h"
#include "BB/Task/UpdateStatusTask.h"

#include <iostream>
#include <Poco/SharedPtr.h>
#include "TBS/Log.h"

#include <Poco/Delegate.h>
#include "BB/Notification.h"

namespace BB {

	class RuntimeStatusManager {
		public:
			Poco::BasicEvent <RuntimeStatus> Changed;

			void updateStatus(const RuntimeStatus & rs){
				std::string oldStatus = RuntimeStatus::toShortString(runtimeStatus);
				runtimeStatus.update(rs);
				std::string newStatus = RuntimeStatus::toShortString(runtimeStatus);
				if (newStatus != oldStatus){
					LINFO("Collector")<< "status changed: " << runtimeStatus << LE;
					Changed.notify(this, runtimeStatus);
				}
			}
		private:
			RuntimeStatus runtimeStatus;

	};

	class DataDistributor: public TBS::BB::Services::Data::IDataDistributor {
		public:
			typedef Poco::SharedPtr<DataDistributor> Ptr;
			void distribute(const BB::SensorData & data) {
				TBS::BB::Services::Data::IDataDistributor::SensorDataReceivedArg a = BB::SensorDataHelpers::sensorData2EventArg(data);
				this->SensorDataReceived(this, a);
			}

			void distribute(const BB::RuntimeStatus & status) {
				TBS::BB::Services::Data::IDataDistributor::StatusReceivedArg a = BB::RuntimeStatus::toShortString(status);
				this->StatusReceived(this, a);
			}

			void distribute(const BB::Task & task) {
				TBS::BB::Services::Data::IDataDistributor::TaskReceivedArg a;
				a.source = task.getSource();
				a.destination = task.getDestination();
				a.what = task.getWhat();
				a.params = task.getParams();
				this->TaskReceived(this, a);
			}

			void distribute(const BB::Notification & ntf) {
				TBS::BB::Services::Data::IDataDistributor::NotificationReceivedArg a;
				a.level = ntf.getLevel();
				a.source = ntf.getSource();
				a.message = ntf.getMessage();
				this->NotificationReceived(this, a);
			}
	};

	class DataCollector: public TBS::BB::Services::Data::IDataCollector {
		public:

			DataCollector(DataDistributor::Ptr distributor) :
					dist(distributor) {
				statusManager.Changed += Poco::delegate(this, &DataCollector::onStatusChanged);
			}
			~DataCollector(){
				statusManager.Changed -= Poco::delegate(this, &DataCollector::onStatusChanged);
			}
			void onStatusChanged(RuntimeStatus & s){
				dist->distribute(s);
			}

			virtual void SendSensorData(const std::string & sensorType, const std::string & sensorName, const std::string & sensorRawName,
					const std::string & sensorUnit, const std::string & sensorDate, const int32_t & sensorStatus, const double & sensorValue,
					const std::string & sensorTextValue) {

				BB::SensorData d(sensorType, sensorName, sensorRawName, sensorUnit, BB::SensorData::string2date(sensorDate),
						(BB::SensorData::Status) sensorStatus, sensorValue, sensorTextValue);

				std::cout << "data received: " << d << std::endl;
				dist->distribute(d);
			}

			virtual void SendStatus(const std::string & status) {
				RuntimeStatus rs = RuntimeStatus::fromShortString(status);
				std::cout << "status received: " << rs << std::endl;
				LINFO("Collector")<< "status received: " << rs << LE;
				//dist->distribute(rs); - is done in manager
				statusManager.updateStatus(rs);
			}

			virtual void SendTask(const std::string & what, const std::string & params, const std::string & from, const std::string & to) {
				Task task(what, params, from, to);
				LINFO("Collector")<< "task received: " << task << LE;
				if (task.getWhat() == Task::UpdateStatus){
					UpdateStatusTask updatestatus(task);
					statusManager.updateStatus(updatestatus.getStatus());
				} else {
					dist->distribute(task);
				}
			}

	        virtual void SendNotification(const int32_t & level, const std::string & source, const std::string & message){
	        	Notification ntf((Notification::Level)level, source, message);
	        	LINFO("Collector")<< "ntf received: " << ntf << LE;
	    		dist->distribute(ntf);
	        }


		private:
			DataDistributor::Ptr dist;
			RuntimeStatusManager statusManager;
	};

	class CollectorService: public Poco::Util::ServerApplication {

		public:

			typedef Poco::SharedPtr<CollectorService> Ptr;
			CollectorService();
			virtual ~CollectorService() {

			}

		protected:

			int main(const std::vector<std::string>& args);

	};

}

#endif /* COLLECTORSERVICE_H_ */
