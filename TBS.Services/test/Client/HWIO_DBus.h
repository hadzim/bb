/**
 * THIS FILE IS GENERATED BY genTool, DO NOT CHANGE IT!!!!!
 */
#ifndef _HWIO_DBUSCLIENT_H_
#define _HWIO_DBUSCLIENT_H_

#include <Poco/SharedPtr.h>
#include <Poco/BasicEvent.h>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <dbus-c++/dbus.h>
///includes
#include "HWIO.h"


namespace TBS { 
	namespace TSeries { 
		namespace Services { 
			namespace HWIO { 
				namespace Stub { 
       class Services_DBusClient : public ::DBus::InterfaceProxy, public TBS::TSeries::Services::HWIO::IServices {
			public:
				typedef Poco::SharedPtr <Services_DBusClient> Ptr;
				
				Services_DBusClient() : ::DBus::InterfaceProxy("com.TBS.TSeries.Services.HWIO.Services"){
					
				}
				virtual ~Services_DBusClient() {
				}

				
				static const std::string & dbuspath(){ static std::string val = "/com/TBS/TSeries/Services/HWIO"; return val; }
				static const std::string & dbusname(){ static std::string val = "com.TBS.TSeries.Services.HWIO.Services"; return val; }
				
				
 //methods 
				virtual std::vector< std::string > Available(){		::DBus::CallMessage call;
		call.member("Available");
		::DBus::Message ret = invoke_method (call);
		::DBus::MessageIter ri = ret.reader();

		std::vector< std::string > _argout;
		ri >> _argout;
		return _argout;
	}

virtual void Reload(){		::DBus::CallMessage call;
		call.member("Reload");
		::DBus::Message ret = invoke_method (call);
	}


		private: 
				
			};
 } 
 } 
 } 
 } 
 } 


namespace TBS { 
	namespace TSeries { 
		namespace Services { 
			namespace HWIO { 
				namespace Stub { 
       class SmartController_DBusClient : public ::DBus::InterfaceProxy, public TBS::TSeries::Services::HWIO::ISmartController {
			public:
				typedef Poco::SharedPtr <SmartController_DBusClient> Ptr;
				
				SmartController_DBusClient() : ::DBus::InterfaceProxy("com.TBS.TSeries.Services.HWIO.SmartController"){
							connect_signal(SmartController_DBusClient, GPIn1Changed, _GPIn1Changed_sigstub);
		connect_signal(SmartController_DBusClient, GPIn2Changed, _GPIn2Changed_sigstub);

				}
				virtual ~SmartController_DBusClient() {
				}

				
				static const std::string & dbuspath(){ static std::string val = "/com/TBS/TSeries/Services/HWIO"; return val; }
				static const std::string & dbusname(){ static std::string val = "com.TBS.TSeries.Services.HWIO.SmartController"; return val; }
				
				
 //methods 
				virtual void Relay1(const int32_t & timeout){		::DBus::CallMessage call;
		::DBus::MessageIter wi = call.writer();

		wi << timeout;
		call.member("Relay1");
		::DBus::Message ret = invoke_method (call);
	}

virtual void Relay2(const int32_t & timeout){		::DBus::CallMessage call;
		::DBus::MessageIter wi = call.writer();

		wi << timeout;
		call.member("Relay2");
		::DBus::Message ret = invoke_method (call);
	}

virtual void GPOut1(const int32_t & status){		::DBus::CallMessage call;
		::DBus::MessageIter wi = call.writer();

		wi << status;
		call.member("GPOut1");
		::DBus::Message ret = invoke_method (call);
	}

virtual void GPOut2(const int32_t & status){		::DBus::CallMessage call;
		::DBus::MessageIter wi = call.writer();

		wi << status;
		call.member("GPOut2");
		::DBus::Message ret = invoke_method (call);
	}

virtual int32_t GPIn1Value(){		::DBus::CallMessage call;
		call.member("GPIn1Value");
		::DBus::Message ret = invoke_method (call);
		::DBus::MessageIter ri = ret.reader();

		int32_t _argout;
		ri >> _argout;
		return _argout;
	}

virtual int32_t GPIn2Value(){		::DBus::CallMessage call;
		call.member("GPIn2Value");
		::DBus::Message ret = invoke_method (call);
		::DBus::MessageIter ri = ret.reader();

		int32_t _argout;
		ri >> _argout;
		return _argout;
	}


		private: 
					void _GPIn1Changed_sigstub(const ::DBus::SignalMessage &sig){
		::DBus::MessageIter ri = sig.reader();

		int32_t _value;
		ri >> _value;
		int32_t _oldvalue;
		ri >> _oldvalue;
		GPIn1ChangedArg sig_arg;
		sig_arg.value = _value;
		sig_arg.oldvalue = _oldvalue;

		//raise poco event
		GPIn1Changed(this, sig_arg);
	}
	void _GPIn2Changed_sigstub(const ::DBus::SignalMessage &sig){
		::DBus::MessageIter ri = sig.reader();

		int32_t _value;
		ri >> _value;
		GPIn2ChangedArg sig_arg;
		sig_arg = _value;

		//raise poco event
		GPIn2Changed(this, sig_arg);
	}

			};
 } 
 } 
 } 
 } 
 } 


namespace TBS { 
	namespace TSeries { 
		namespace Services { 
			namespace HWIO { 
				namespace Stub { 
       class InternalRelay_DBusClient : public ::DBus::InterfaceProxy, public TBS::TSeries::Services::HWIO::IInternalRelay {
			public:
				typedef Poco::SharedPtr <InternalRelay_DBusClient> Ptr;
				
				InternalRelay_DBusClient() : ::DBus::InterfaceProxy("com.TBS.TSeries.Services.HWIO.InternalRelay"){
					
				}
				virtual ~InternalRelay_DBusClient() {
				}

				
				static const std::string & dbuspath(){ static std::string val = "/com/TBS/TSeries/Services/HWIO"; return val; }
				static const std::string & dbusname(){ static std::string val = "com.TBS.TSeries.Services.HWIO.InternalRelay"; return val; }
				
				
 //methods 
				virtual void Relay(const int32_t & timeoutMs){		::DBus::CallMessage call;
		::DBus::MessageIter wi = call.writer();

		wi << timeoutMs;
		call.member("Relay");
		::DBus::Message ret = invoke_method (call);
	}


		private: 
				
			};
 } 
 } 
 } 
 } 
 } 



#endif //_HWIO_DBUSCLIENT_H_
