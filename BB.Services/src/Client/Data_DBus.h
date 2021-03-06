/**
 * THIS FILE IS GENERATED BY genTool, DO NOT CHANGE IT!!!!!
 */
#ifndef _NO_DBUS 
#ifndef _DATA_DBUSCLIENT_H_
#define _DATA_DBUSCLIENT_H_

#include <Poco/SharedPtr.h>
#include <Poco/BasicEvent.h>
#include <Poco/Exception.h>
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <memory>
#include <dbus-c++/dbus.h>
#include <dbus/dbus-protocol.h>
///includes
#include "BB/Services//Data.h"


namespace TBS { 
	namespace BB { 
		namespace Services { 
			namespace Data { 
				namespace Stub { 
       class DataCollector_DBusClient : public ::DBus::InterfaceProxy, public TBS::BB::Services::Data::IDataCollector {
			public:
				typedef Poco::SharedPtr <DataCollector_DBusClient> Ptr;
				
				DataCollector_DBusClient() : ::DBus::InterfaceProxy("com.TBS.BB.Services.Data.DataCollector"){
					
				}
				virtual ~DataCollector_DBusClient() {
				}

				
				static const std::string & dbuspath(){ static std::string val = "/com/TBS/BB/Services/Data"; return val; }
				static const std::string & dbusname(){ static std::string val = "com.TBS.BB.Services.Data.DataCollector"; return val; }
				
				
 //methods 
				virtual void SendSensorData(const std::string & sensorType, const std::string & sensorName, const std::string & sensorRawName, const std::string & sensorUnit, const std::string & sensorDate, const int32_t & sensorStatus, const double & sensorValue, const std::string & sensorTextValue){		try {
		::DBus::CallMessage call;
		::DBus::MessageIter wi = call.writer();

		wi << sensorType;
		wi << sensorName;
		wi << sensorRawName;
		wi << sensorUnit;
		wi << sensorDate;
		wi << sensorStatus;
		wi << sensorValue;
		wi << sensorTextValue;
		call.member("SendSensorData");
		::DBus::Message ret = invoke_method (call);
		} catch (::DBus::Error & err){
			if (strcmp(err.name(), DBUS_ERROR_FAILED) == 0){
				throw ::TBS::Services::RuntimeServiceException(err.message());
			} else {
				throw ::TBS::Services::ChannelServiceException(err.what());
			}
		}
	}

virtual void SendStatus(const std::string & status){		try {
		::DBus::CallMessage call;
		::DBus::MessageIter wi = call.writer();

		wi << status;
		call.member("SendStatus");
		::DBus::Message ret = invoke_method (call);
		} catch (::DBus::Error & err){
			if (strcmp(err.name(), DBUS_ERROR_FAILED) == 0){
				throw ::TBS::Services::RuntimeServiceException(err.message());
			} else {
				throw ::TBS::Services::ChannelServiceException(err.what());
			}
		}
	}

virtual void SendTask(const std::string & what, const std::string & params, const std::string & source, const std::string & destination){		try {
		::DBus::CallMessage call;
		::DBus::MessageIter wi = call.writer();

		wi << what;
		wi << params;
		wi << source;
		wi << destination;
		call.member("SendTask");
		::DBus::Message ret = invoke_method (call);
		} catch (::DBus::Error & err){
			if (strcmp(err.name(), DBUS_ERROR_FAILED) == 0){
				throw ::TBS::Services::RuntimeServiceException(err.message());
			} else {
				throw ::TBS::Services::ChannelServiceException(err.what());
			}
		}
	}

virtual void SendNotification(const int32_t & level, const std::string & source, const std::string & message){		try {
		::DBus::CallMessage call;
		::DBus::MessageIter wi = call.writer();

		wi << level;
		wi << source;
		wi << message;
		call.member("SendNotification");
		::DBus::Message ret = invoke_method (call);
		} catch (::DBus::Error & err){
			if (strcmp(err.name(), DBUS_ERROR_FAILED) == 0){
				throw ::TBS::Services::RuntimeServiceException(err.message());
			} else {
				throw ::TBS::Services::ChannelServiceException(err.what());
			}
		}
	}


		private: 
				
			};
 } 
 } 
 } 
 } 
 } 


namespace TBS { 
	namespace BB { 
		namespace Services { 
			namespace Data { 
				namespace Stub { 
       class DataDistributor_DBusClient : public ::DBus::InterfaceProxy, public TBS::BB::Services::Data::IDataDistributor {
			public:
				typedef Poco::SharedPtr <DataDistributor_DBusClient> Ptr;
				
				DataDistributor_DBusClient() : ::DBus::InterfaceProxy("com.TBS.BB.Services.Data.DataDistributor"){
							connect_signal(DataDistributor_DBusClient, SensorDataReceived, _SensorDataReceived_sigstub);
		connect_signal(DataDistributor_DBusClient, StatusReceived, _StatusReceived_sigstub);
		connect_signal(DataDistributor_DBusClient, TaskReceived, _TaskReceived_sigstub);
		connect_signal(DataDistributor_DBusClient, NotificationReceived, _NotificationReceived_sigstub);

				}
				virtual ~DataDistributor_DBusClient() {
				}

				
				static const std::string & dbuspath(){ static std::string val = "/com/TBS/BB/Services/Data"; return val; }
				static const std::string & dbusname(){ static std::string val = "com.TBS.BB.Services.Data.DataDistributor"; return val; }
				
				
 //methods 
				
		private: 
					void _SensorDataReceived_sigstub(const ::DBus::SignalMessage &sig){
		::DBus::MessageIter ri = sig.reader();

		std::string _sensorType;
		ri >> _sensorType;
		std::string _sensorName;
		ri >> _sensorName;
		std::string _sensorRawName;
		ri >> _sensorRawName;
		std::string _sensorUnit;
		ri >> _sensorUnit;
		std::string _sensorDate;
		ri >> _sensorDate;
		int32_t _sensorStatus;
		ri >> _sensorStatus;
		double _sensorValue;
		ri >> _sensorValue;
		std::string _sensorTextValue;
		ri >> _sensorTextValue;
		SensorDataReceivedArg sig_arg;
		sig_arg.sensorType = _sensorType;
		sig_arg.sensorName = _sensorName;
		sig_arg.sensorRawName = _sensorRawName;
		sig_arg.sensorUnit = _sensorUnit;
		sig_arg.sensorDate = _sensorDate;
		sig_arg.sensorStatus = _sensorStatus;
		sig_arg.sensorValue = _sensorValue;
		sig_arg.sensorTextValue = _sensorTextValue;

		//raise poco event
		SensorDataReceived(this, sig_arg);
	}
	void _StatusReceived_sigstub(const ::DBus::SignalMessage &sig){
		::DBus::MessageIter ri = sig.reader();

		std::string _status;
		ri >> _status;
		StatusReceivedArg sig_arg;
		sig_arg = _status;

		//raise poco event
		StatusReceived(this, sig_arg);
	}
	void _TaskReceived_sigstub(const ::DBus::SignalMessage &sig){
		::DBus::MessageIter ri = sig.reader();

		std::string _what;
		ri >> _what;
		std::string _params;
		ri >> _params;
		std::string _source;
		ri >> _source;
		std::string _destination;
		ri >> _destination;
		TaskReceivedArg sig_arg;
		sig_arg.what = _what;
		sig_arg.params = _params;
		sig_arg.source = _source;
		sig_arg.destination = _destination;

		//raise poco event
		TaskReceived(this, sig_arg);
	}
	void _NotificationReceived_sigstub(const ::DBus::SignalMessage &sig){
		::DBus::MessageIter ri = sig.reader();

		int32_t _level;
		ri >> _level;
		std::string _source;
		ri >> _source;
		std::string _message;
		ri >> _message;
		NotificationReceivedArg sig_arg;
		sig_arg.level = _level;
		sig_arg.source = _source;
		sig_arg.message = _message;

		//raise poco event
		NotificationReceived(this, sig_arg);
	}

			};
 } 
 } 
 } 
 } 
 } 



#endif //_DATA_DBUSCLIENT_H_
#endif

