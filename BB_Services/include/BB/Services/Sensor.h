/**
 * THIS FILE IS GENERATED BY genTool, DO NOT CHANGE IT!!!!!
 */
#ifndef _SENSOR_H_
#define _SENSOR_H_

#include <Poco/SharedPtr.h>
#include <Poco/BasicEvent.h>
#include "TBS/Services/Types.h"
#include <string>
#include <vector>
#include <map>

namespace TBS{ namespace Services{ namespace Introspection{ class Class; struct Struct; } } } 




namespace TBS { 
	namespace BB { 
		namespace Services { 
			namespace Sensor { 
	   /** [interface] IDataCollector:  */
       class IDataCollector {
			public:
				typedef Poco::SharedPtr <IDataCollector> Ptr;
				
				virtual ~IDataCollector(){}
				
				static std::string name(){
					static std::string n = "TBS.BB.Services.Sensor.DataCollector";
					return n;
				}
				static const ::TBS::Services::Introspection::Class & introspection();
				
 //methods 
				
		/** 
		  * [method] SendSensorData: 
		  * [in] std::string sensorType: 
		  * [in] std::string sensorName: 
		  * [in] std::string sensorUnit: 
		  * [in] std::string sensorDate: 
		  * [in] int32_t sensorStatus: 
		  * [in] double sensorValue: 
		  * [in] std::string sensorTextValue: 
		  */ 
		        virtual void SendSensorData(const std::string & sensorType, const std::string & sensorName, const std::string & sensorUnit, const std::string & sensorDate, const int32_t & sensorStatus, const double & sensorValue, const std::string & sensorTextValue) = 0;

				
 //signals 
				
				
			};
 } 
 } 
 } 
 } 


namespace TBS { 
	namespace BB { 
		namespace Services { 
			namespace Sensor { 
	   /** [interface] IDataDistributor:  */
       class IDataDistributor {
			public:
				typedef Poco::SharedPtr <IDataDistributor> Ptr;
				
				virtual ~IDataDistributor(){}
				
				static std::string name(){
					static std::string n = "TBS.BB.Services.Sensor.DataDistributor";
					return n;
				}
				static const ::TBS::Services::Introspection::Class & introspection();
				
 //methods 
				
				
 //signals 
						struct SensorDataReceivedArg { 
std::string sensorType;
std::string sensorName;
std::string sensorUnit;
std::string sensorDate;
int32_t sensorStatus;
double sensorValue;
std::string sensorTextValue;
};
		Poco::BasicEvent <SensorDataReceivedArg> SensorDataReceived;
		

				
			};
 } 
 } 
 } 
 } 



#endif //_SENSOR_H_

