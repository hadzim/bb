/*
 * DataSender.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: dron
 */

#include <BB/Service/DataSender2.h>

#include "TBS/MQTT/Client.h"

#include "BB/DataRW.h"
namespace BB {


	class MQTTDataSender : public IDataSender2 {
		public:
			MQTTDataSender(std::string name, std::string project, std::string url = "localhost")
				: project(project), client(name, url){

			}

			void send(const std::string & sensorUID, const Sensor::Data & data){
				client.publish(
						"devices/" + project + "/controls/" + sensorUID + "/data/current",
						RW::json2String(Sensor2DataRW::write(data))
				);
			}
			void send(const Sensor::Info & info){
				client.publish(
						"devices/" + project + "/controls/" + info.getUID() + "/type",
						info.getType()
				);
				client.publish(
						"devices/" + project + "/controls/" + info.getUID() + "/data/unit",
						info.getUnit()
				);
			}

		private:
			std::string project;
			TBS::MQTT::Client client;
	};

	IDataSender2::Ptr IDataSender2::create(std::string name, std::string projectID){
		return new MQTTDataSender(name, projectID);
	}
	IDataSender2::Ptr IDataSender2::createRemote(std::string name, std::string projectID, std::string serverAddress){
		return new MQTTDataSender(name, projectID, serverAddress);
	}


} /* namespace BB */
