/*
 * HttpServerForwarder.cpp
 *
 *  Created on: 2.4.2014
 *      Author: JV
 */

#include "BB/Forwarder/HttpServer/HttpServerForwarder.h"
#include "BB/Sensor/SensorDataHelpers.h"
#include "BB/Configuration.h"

#include "TBS/Log.h"

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
		public:
			typedef TBS::BB::WebUI::SensorProperty Property;
			typedef std::vector<Property> Properties;

			virtual std::vector< Property > GetSensorProperties(const std::string & sensorType, const std::string & sensorRawName){
				throw Poco::Exception("not implemented yet");
			}


			virtual void SetSensorProperty(const std::string & sensorType, const std::string & sensorRawName, const std::string & sensorPropertyName, const std::string & sensorPropertyValue){
				BB::Configuration::setSensorProperty(sensorType, sensorRawName, sensorPropertyName, sensorPropertyValue);
			}

			virtual void SetRuntimeStatus(const std::string & status) {
				throw Poco::Exception("not implemented yet");
			}

			virtual void SendTask(const std::string & what, const std::string & params, const std::string & from, const std::string & to){
				throw Poco::Exception("not implemented");
			}

	};

	class QueryProvider: public TBS::BB::WebUI::IQuery, public IForwarder {
			Poco::Mutex mtx;

			IFilter::PtrList filters;
			std::size_t oneSensorLimit;

			typedef std::map<std::string, BB::SensorData> SensorDataMap;
			typedef std::map<std::string, SensorDataMap> Storage;
			typedef std::map<std::string, TBS::BB::WebUI::SensorInfo> Sensors;

			Sensors sensors;
			Storage storage;
			SensorDataMap current;
			RuntimeStatus runtimeStatus;
		public:
			typedef Poco::SharedPtr<QueryProvider> Ptr;

			QueryProvider(const IFilter::PtrList & filters, int oneSensorLimit) :
					filters(filters), oneSensorLimit(oneSensorLimit) {

			}
			virtual ~QueryProvider() {

			}

			void clear(std::string name){

			}

			//idea: push data with filter
			//+ keep current
			void forward(const BB::SensorData & sd) {
				try {
					Poco::Mutex::ScopedLock l(mtx);

					//std::cout << "accept " << sd << std::endl;

					std::string key = SensorDataHelpers::sensorID(sd);

					sensors[key].sensorRawName = sd.getRawName();
					sensors[key].sensorName = sd.getName();
					sensors[key].sensorType = sd.getType();

					if (current.find(key) != current.end()) {
						current.erase(key);
					}
					current.insert(std::make_pair(key, sd));

					SensorDataMap & m = storage[key];

					TBS::Nullable<SensorData> data;
					for (IFilter::PtrList::iterator i = filters.begin(); i != filters.end(); i++) {
						TBS::Nullable<SensorData> fdata = (*i)->filter(sd);
						if (fdata.isSet()) {
							std::cout << "filter pass " << fdata << std::endl;
							data = fdata;
						}
					}

					if (data.isSet()) {

						LINFO("HttpServer")<< "SensorData: " << data.ref() << LE;
					} else {
						std::cout << "no filter active for " << sd << std::endl;;
					}

					if (data.isSet()) {

						m.erase(sensorDateKey(data.ref()));
						m.insert(std::pair<std::string, BB::SensorData>(sensorDateKey(data.ref()), data.ref()));
					} else {
						//insert as current
						std::cout << "insert current only: " << sd << std::endl;
						current.erase(key);
						current.insert(std::make_pair(key, sd));
					}

					while (m.size() > oneSensorLimit) {
						m.erase(m.begin());
					}
				} catch (Poco::Exception & e) {
					std::cerr << "EXC: " << e.displayText() << std::endl;
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

			virtual std::vector<TBS::BB::WebUI::SensorData> GetSensorData(const std::string & sensorType, const std::string & sensorName) {

				Poco::Mutex::ScopedLock l(mtx);
				std::vector<TBS::BB::WebUI::SensorData> all;

				std::string key = SensorDataHelpers::sensorID(sensorType, sensorName);

				SensorDataMap & m = storage[key];
				for (SensorDataMap::iterator i = m.begin(); i != m.end(); i++) {
					all.push_back(convertSD(i->second));
				}

				SensorDataMap::iterator c = current.find(key);
				if (c != current.end()) {
					all.push_back(convertSD(c->second));
				}

				return all;
			}

			virtual void ClearSensorData(const std::string & sensorType, const std::string & sensorName){
				Poco::Mutex::ScopedLock l(mtx);

				std::string key = SensorDataHelpers::sensorID(sensorType, sensorName);

				storage.erase(key);
				current.erase(key);
				sensors.erase(key);
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

				for (SensorDataMap::iterator i = current.begin(); i != current.end(); i++) {
					if (i->second.getType() == sensorType) {
						all.push_back(convertSD(i->second));
					}
				}

				return all;
			}

			void forward(const BB::RuntimeStatus & sd) {
				Poco::Mutex::ScopedLock l(mtx);
				this->runtimeStatus = sd;
			}

			void forward(const BB::Task & t) {
				//TODO process tasks
			}
			void forward(const BB::Notification & t) {
				//TODO process notification
			}

			virtual std::vector<TBS::BB::WebUI::RuntimeStatus> GetRuntimeStatus() {
				Poco::Mutex::ScopedLock l(mtx);
				std::vector<TBS::BB::WebUI::RuntimeStatus> rs;

				for (BB::RuntimeStatus::iterator i = this->runtimeStatus.begin(); i != this->runtimeStatus.end(); i++){
					TBS::BB::WebUI::RuntimeStatus val;
					val.index = i->first;
					val.value = i->second;
					val.readableIndex = BB::RuntimeStatus::readableIndex(i->first);
					val.readableValue = BB::RuntimeStatus::readableValue(i->first, i->second);
					rs.push_back(val);
				}

				return rs;
			}
	};

	HttpServerForwarder::HttpServerForwarder(int port, const IFilter::PtrList & filters, int oneSensorLimit) {

		TBS::Services::JsonServerParams ch(port);
		jsonServer = TBS::BB::WebUI::Json::Server::createJsonpServer(ch);

		QueryProvider::Ptr queryProvider = new QueryProvider(filters, oneSensorLimit);
		ConfigurationProvider::Ptr cfgProvider = new ConfigurationProvider();

		this->query = queryProvider;

		sq = jsonServer->createQuery(queryProvider);
		sc = jsonServer->createConfiguration(cfgProvider);

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
	void HttpServerForwarder::forward(const RuntimeStatus & d){
		this->query->forward(d);
	}
	void HttpServerForwarder::forward(const Task & d){
		this->query->forward(d);
	}
	void HttpServerForwarder::forward(const Notification & d){
		this->query->forward(d);
	}

} /* namespace BB */
