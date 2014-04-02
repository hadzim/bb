/*
 * HttpServerForwarder.cpp
 *
 *  Created on: 2.4.2014
 *      Author: JV
 */

#include "BB/Forwarder/HttpServer/HttpServerForwarder.h"
#include "BB/Sensor/SensorDataHelpers.h"
#include "BB/Configuration.h"

namespace BB {

	static std::string sensorDateKey(const BB::SensorData & d) {
		return SensorDataHelpers::sensorID(d) + "::" + BB::SensorData::date2string(d.getDate());
	}

	static TBS::BB::WebUI::SensorData convertSD(const BB::SensorData & data) {
		TBS::BB::WebUI::SensorData sd;
		sd.date = data.getDateAsString();
		sd.sensorName = data.getName();
		sd.sensorRawName = data.getRawName();
		sd.sensorType = data.getType();
		sd.status = (int) data.getSensorStatus();
		sd.unit = data.getUnit();
		sd.value = data.getValue();
		sd.textValue = data.getTextValue();
		return sd;
	}

	class ConfigurationProvider: public TBS::BB::WebUI::IConfiguration {
			void SetSensorProperty(const std::string & sensorType, const std::string & sensorRawName,
					const std::string & sensorProperty, const std::string & sensorValue) {
				BB::Configuration::setSensorProperty(sensorType, sensorRawName, sensorProperty, sensorValue);
			}
	};

	class QueryProvider: public TBS::BB::WebUI::IQuery, public IForwarder {
			Poco::Mutex mtx;
		public:
			typedef Poco::SharedPtr<QueryProvider> Ptr;

			QueryProvider() {

			}
			virtual ~QueryProvider() {

			}

			void forward(const BB::SensorData & sd) {

				Poco::Mutex::ScopedLock l(mtx);

				std::cout << "accept " << sd << std::endl;

				std::string key = SensorDataHelpers::sensorID(sd);

				sensors[key].sensorRawName = sd.getRawName();
				sensors[key].sensorName = sd.getName();
				sensors[key].sensorType = sd.getType();

				SensorDataMap & m = storage[key];

				m.erase(sensorDateKey(sd));

				m.insert(std::pair<std::string, BB::SensorData>(sensorDateKey(sd), sd));

				while (m.size() > 100) {
					m.erase(m.begin());
				}

			}

			virtual std::vector<TBS::BB::WebUI::SensorInfo> GetSensors() {
				Poco::Mutex::ScopedLock l(mtx);
				std::vector<TBS::BB::WebUI::SensorInfo> all;

				for (Sensors::iterator i = sensors.begin(); i != sensors.end(); i++) {
					all.push_back(i->second);
				}

				return all;
			}

			virtual std::vector<TBS::BB::WebUI::SensorData> GetSensorData(const std::string & sensorType,
					const std::string & sensorName) {

				Poco::Mutex::ScopedLock l(mtx);
				std::vector<TBS::BB::WebUI::SensorData> all;

				SensorDataMap & m = storage[SensorDataHelpers::sensorID(sensorType, sensorName)];
				for (SensorDataMap::iterator i = m.begin(); i != m.end(); i++) {

					all.push_back(convertSD(i->second));
				}
				return all;
			}

			virtual std::vector<TBS::BB::WebUI::SensorData> GetSensorsData(const std::string & sensorType) {

				Poco::Mutex::ScopedLock l(mtx);
				std::vector<TBS::BB::WebUI::SensorData> all;

				for (Storage::iterator sit = storage.begin(); sit != storage.end(); sit++) {

					for (SensorDataMap::iterator i = sit->second.begin(); i != sit->second.end(); i++) {
						if (i->second.getType() == sensorType) {
							all.push_back(convertSD(i->second));
						}
					}
				}
				return all;
			}

		private:
			typedef std::map<std::string, BB::SensorData> SensorDataMap;
			typedef std::map<std::string, SensorDataMap> Storage;
			typedef std::map<std::string, TBS::BB::WebUI::SensorInfo> Sensors;
			Sensors sensors;
			Storage storage;
	};

	HttpServerForwarder::HttpServerForwarder(int port) {

		TBS::Services::JsonServerChannel ch(port);
		jsonServer = new TBS::BB::WebUI::Json::Server(ch);

		QueryProvider::Ptr queryProvider = new QueryProvider();
		ConfigurationProvider::Ptr cfgProvider = new ConfigurationProvider();

		this->query = queryProvider;

		TBS::Services::IServer::Ptr sq = jsonServer->createQuery(queryProvider);
		TBS::Services::IServer::Ptr sc = jsonServer->createConfiguration(cfgProvider);

		jsonServer->start();

	}

	HttpServerForwarder::~HttpServerForwarder() {
		sq = NULL;
		sc = NULL;
		query = NULL;
		jsonServer->stop();
	}

	void HttpServerForwarder::forward(const SensorData & d) {
		this->query->forward(d);
	}

} /* namespace BB */