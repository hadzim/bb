/**
 * THIS FILE IS GENERATED BY genTool, DO NOT CHANGE IT!!!!!
 */
#ifndef _VISUALIZATION_H_
#define _VISUALIZATION_H_

#include <Poco/SharedPtr.h>
#include <Poco/BasicEvent.h>
#include "TBS/Services/Types.h"
#include <string>
#include <vector>
#include <map>

namespace TBS{ namespace Services{ namespace Introspection{ class Class; struct Struct; } } } 


namespace TBS { 
	namespace BB { 
		namespace Visualization { 
       struct SensorData {
				
				static std::string name(){
					static std::string n = "SensorData";
					return n;
				}
				static const ::TBS::Services::Introspection::Struct & introspection();
				
 //members 
						std::string sensorType;
		std::string sensorName;
		std::string unit;
		std::string date;
		int32_t status;
		double value;
		std::string textValue;

				
			};
 } 
 } 
 } 


namespace TBS { 
	namespace BB { 
		namespace Visualization { 
       struct SensorInfo {
				
				static std::string name(){
					static std::string n = "SensorInfo";
					return n;
				}
				static const ::TBS::Services::Introspection::Struct & introspection();
				
 //members 
						std::string sensorType;
		std::string sensorName;

				
			};
 } 
 } 
 } 




namespace TBS { 
	namespace BB { 
		namespace Visualization { 
	   /** [interface] IQuery:  */
       class IQuery {
			public:
				typedef Poco::SharedPtr <IQuery> Ptr;
				
				virtual ~IQuery(){}
				
				static std::string name(){
					static std::string n = "TBS.BB.Visualization.Query";
					return n;
				}
				static const ::TBS::Services::Introspection::Class & introspection();
				
 //methods 
				
		/** 
		  * [method] GetSensors: 
		  * [out] std::vector< SensorInfo > sensors: 
		  */ 
		        virtual std::vector< SensorInfo > GetSensors() = 0;

		/** 
		  * [method] GetSensorData: 
		  * [in] std::string sensorType: 
		  * [in] std::string sensorName: 
		  * [out] std::vector< SensorData > sensorData: 
		  */ 
		        virtual std::vector< SensorData > GetSensorData(const std::string & sensorType, const std::string & sensorName) = 0;

		/** 
		  * [method] GetSensorsData: 
		  * [in] std::string sensorType: 
		  * [out] std::vector< SensorData > allSensorData: 
		  */ 
		        virtual std::vector< SensorData > GetSensorsData(const std::string & sensorType) = 0;

				
 //signals 
				
				
			};
 } 
 } 
 } 



#endif //_VISUALIZATION_H_
