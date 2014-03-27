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
#include "BB/Services/Sensor.h"

#include "BB/Sensor/SensorData.h"
#include "BB/Sensor/SensorDataHelpers.h"

#include <iostream>
#include <Poco/SharedPtr.h>

namespace BB {

class SensorDistributor: public TBS::BB::Services::Sensor::IDataDistributor {
public:
	typedef Poco::SharedPtr <SensorDistributor> Ptr;
	void distribute(const BB::SensorData & data){
		TBS::BB::Services::Sensor::IDataDistributor::SensorDataReceivedArg a = BB::SensorDataHelpers::sensorData2EventArg(data);
		this->SensorDataReceived(this, a);
	}
};

class SensorCollector: public TBS::BB::Services::Sensor::IDataCollector {
public:

	SensorCollector(SensorDistributor::Ptr distributor) : dist(distributor){

	}

	virtual void SendSensorData(const std::string & sensorType,
				const std::string & sensorName, const std::string & sensorUnit,
				const std::string & sensorDate, const int32_t & sensorStatus,
				const double & sensorValue, const std::string & sensorTextValue){

		BB::SensorData d(
				sensorType,
				sensorName,
				sensorUnit,
				BB::SensorData::string2date(sensorDate),
				(BB::SensorData::Status)sensorStatus,
				sensorValue,
				sensorTextValue
		);


		std::cout << "data received: " << d << std::endl;
		dist->distribute(d);
	}
private:
	SensorDistributor::Ptr dist;
};

class CollectorService: public Poco::Util::ServerApplication {

public:

	class Cfg: public Configuration {
	public:
		Cfg();
		std::string getNickname(std::string originalName);
	private:
		Poco::AutoPtr<Poco::Util::ConfigurationView> nicknames;
	};

	typedef Poco::SharedPtr<CollectorService> Ptr;
	CollectorService();
	virtual ~CollectorService() {

	}

protected:

	int main(const std::vector<std::string>& args);

private:
	Cfg cfg;

};

}

#endif /* COLLECTORSERVICE_H_ */
