/*
 * DataSender.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: dron
 */

#include <BB/Service/DataSender.h>
#include <BB/Sensor/SensorDataHelpers.h>
namespace BB {

	DataSender::DataSender() {

	}

	DataSender::~DataSender() {

	}

	void DataSender::send(const SensorData & d) {
		SensorDataHelpers::sendData(client.DataCollector(), d);
	}
	void DataSender::send(const RuntimeStatus & rs) {
		client.DataCollector().SendStatus(RuntimeStatus::toShortString(rs));
	}
	void DataSender::send(const Notification & ntf) {
		client.DataCollector().SendNotification(ntf.getLevel(), ntf.getSource(), ntf.getMessage());
	}
	void DataSender::send(const Task & task) {
		client.DataCollector().SendTask(task.getWhat(), task.getParams(), task.getSource(), task.getDestination());
	}

} /* namespace BB */
