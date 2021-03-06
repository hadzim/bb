/**
 * THIS FILE IS GENERATED BY genTool, DO NOT CHANGE IT!!!!!
 */
#include "TBS/Services/Introspection.h"
#include "BB/Services//Data.h"



namespace TBS { 
	namespace BB { 
		namespace Services { 
			namespace Data { 
		::TBS::Services::Introspection::Class __introspectionIDataCollector(){
			::TBS::Services::Introspection::Namespace cnmspc;
cnmspc.push_back("TBS");
cnmspc.push_back("BB");
cnmspc.push_back("Services");
cnmspc.push_back("Data");
::TBS::Services::Introspection::Class c("TBS.BB.Services.Data.DataCollector", "DataCollector", cnmspc,"");
{
	::TBS::Services::Introspection::Method m("SendSensorData", "");
  m.inArguments.push_back(::TBS::Services::Introspection::Argument("sensorType", "s", ""));
  m.inArguments.push_back(::TBS::Services::Introspection::Argument("sensorName", "s", ""));
  m.inArguments.push_back(::TBS::Services::Introspection::Argument("sensorRawName", "s", ""));
  m.inArguments.push_back(::TBS::Services::Introspection::Argument("sensorUnit", "s", ""));
  m.inArguments.push_back(::TBS::Services::Introspection::Argument("sensorDate", "s", ""));
  m.inArguments.push_back(::TBS::Services::Introspection::Argument("sensorStatus", "i", ""));
  m.inArguments.push_back(::TBS::Services::Introspection::Argument("sensorValue", "d", ""));
  m.inArguments.push_back(::TBS::Services::Introspection::Argument("sensorTextValue", "s", ""));
	c.methods.push_back(m);
}
{
	::TBS::Services::Introspection::Method m("SendStatus", "");
  m.inArguments.push_back(::TBS::Services::Introspection::Argument("status", "s", ""));
	c.methods.push_back(m);
}
{
	::TBS::Services::Introspection::Method m("SendTask", "");
  m.inArguments.push_back(::TBS::Services::Introspection::Argument("what", "s", ""));
  m.inArguments.push_back(::TBS::Services::Introspection::Argument("params", "s", ""));
  m.inArguments.push_back(::TBS::Services::Introspection::Argument("source", "s", ""));
  m.inArguments.push_back(::TBS::Services::Introspection::Argument("destination", "s", ""));
	c.methods.push_back(m);
}
{
	::TBS::Services::Introspection::Method m("SendNotification", "");
  m.inArguments.push_back(::TBS::Services::Introspection::Argument("level", "i", ""));
  m.inArguments.push_back(::TBS::Services::Introspection::Argument("source", "s", ""));
  m.inArguments.push_back(::TBS::Services::Introspection::Argument("message", "s", ""));
	c.methods.push_back(m);
}
return c;

		}
	   const ::TBS::Services::Introspection::Class & IDataCollector::introspection(){
			static ::TBS::Services::Introspection::Class c(__introspectionIDataCollector());
			return c;
	   }
 } 
 } 
 } 
 } 


namespace TBS { 
	namespace BB { 
		namespace Services { 
			namespace Data { 
		::TBS::Services::Introspection::Class __introspectionIDataDistributor(){
			::TBS::Services::Introspection::Namespace cnmspc;
cnmspc.push_back("TBS");
cnmspc.push_back("BB");
cnmspc.push_back("Services");
cnmspc.push_back("Data");
::TBS::Services::Introspection::Class c("TBS.BB.Services.Data.DataDistributor", "DataDistributor", cnmspc,"");
{
	::TBS::Services::Introspection::Signal s("SensorDataReceived", "");
  s.arguments.push_back(::TBS::Services::Introspection::Argument("sensorType", "s", ""));
  s.arguments.push_back(::TBS::Services::Introspection::Argument("sensorName", "s", ""));
  s.arguments.push_back(::TBS::Services::Introspection::Argument("sensorRawName", "s", ""));
  s.arguments.push_back(::TBS::Services::Introspection::Argument("sensorUnit", "s", ""));
  s.arguments.push_back(::TBS::Services::Introspection::Argument("sensorDate", "s", ""));
  s.arguments.push_back(::TBS::Services::Introspection::Argument("sensorStatus", "i", ""));
  s.arguments.push_back(::TBS::Services::Introspection::Argument("sensorValue", "d", ""));
  s.arguments.push_back(::TBS::Services::Introspection::Argument("sensorTextValue", "s", ""));
	c.signals.push_back(s);
}
{
	::TBS::Services::Introspection::Signal s("StatusReceived", "");
  s.arguments.push_back(::TBS::Services::Introspection::Argument("status", "s", ""));
	c.signals.push_back(s);
}
{
	::TBS::Services::Introspection::Signal s("TaskReceived", "");
  s.arguments.push_back(::TBS::Services::Introspection::Argument("what", "s", ""));
  s.arguments.push_back(::TBS::Services::Introspection::Argument("params", "s", ""));
  s.arguments.push_back(::TBS::Services::Introspection::Argument("source", "s", ""));
  s.arguments.push_back(::TBS::Services::Introspection::Argument("destination", "s", ""));
	c.signals.push_back(s);
}
{
	::TBS::Services::Introspection::Signal s("NotificationReceived", "");
  s.arguments.push_back(::TBS::Services::Introspection::Argument("level", "i", ""));
  s.arguments.push_back(::TBS::Services::Introspection::Argument("source", "s", ""));
  s.arguments.push_back(::TBS::Services::Introspection::Argument("message", "s", ""));
	c.signals.push_back(s);
}
return c;

		}
	   const ::TBS::Services::Introspection::Class & IDataDistributor::introspection(){
			static ::TBS::Services::Introspection::Class c(__introspectionIDataDistributor());
			return c;
	   }
 } 
 } 
 } 
 } 




