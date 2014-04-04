#ifndef CLIENTTEMPLATE_CLIENT_H_
#define CLIENTTEMPLATE_CLIENT_H_
/*
#define TEMPLATE_CLIENT_METHOD "\
        <returnType> <methodName>(<parameters>){\n\
            DBus::Value p;\n\
            <parameterAssign>\n\
            <returnStatement>\n\
        }\n\
"
*/



#define TEMPLATE_STUB "\
/**\n\
 * THIS FILE IS GENERATED BY genTool, DO NOT CHANGE IT!!!!!\n\
 */\n\
 #ifndef _NO_DBUS \n\
#ifndef _<STUBNAME>_SERVICE_DBUS_H_\n\
#define _<STUBNAME>_SERVICE_DBUS_H_\n\
\n\
#include <Poco/SharedPtr.h>\n\
#include <Poco/BasicEvent.h>\n\
#include <string>\n\
#include <vector>\n\
#include <map>\n\
#include <memory>\n\
#include <TBS/Services/Services.h>\n\
#include <TBS/Services/DBus/DBusServices.h>\n\
///includes\n\
<includes>\n\
\n\
namespace TBS {\n\
	namespace Services {\n\
		class CommunicationChannelHolder;\n\
	}\n\
}\n\
\n\
<namespaceStart>\
       class GEN_SERVICE_API Client {\n\
			public:\n\
				typedef Poco::SharedPtr <Client> Ptr;\n\
				\n\
				//with given dbus dispatcher via DBusCommChannelProvider\n\
				Client(TBS::Services::ICommChannelHolder::Ptr ch);\n\
				//with default dbus dispatcher\n\
				Client();\n\
				~Client();\n\
				\n\
				<clients>\n\
				\n\
		private: \n\
				TBS::Services::ICommChannelHolder::Ptr ch;\n\
				std::auto_ptr<TBS::Services::CommunicationChannelHolder> comChannelHolder;\n\
				<private>\n\
			};\n\
			\n\
			\n\
		class GEN_SERVICE_API Server {\n\
			public:\n\
				typedef Poco::SharedPtr<Server> Ptr;\n\
				//with given dbus dispatcher via DBusCommChannelProvider\n\
				Server(TBS::Services::ICommChannelHolder::Ptr ch);\n\
				//uses default dbus dispatcher\n\
				Server();\n\
				~Server();\n\
				\n\
			public:\n\
				<servers>\n\
				\n\
			private:\n\
				TBS::Services::ICommChannelHolder::Ptr ch;\n\
		};\n\
<namespaceEnd>\n\
#endif //_<STUBNAME>_SERVICE_H_\n\
#endif\n\
"


#define TEMPLATE_CPP_STUB "\
#ifndef _NO_DBUS \n\
/**\n\
 * THIS FILE IS GENERATED BY genTool, DO NOT CHANGE IT!!!!!\n\
 */\n\
///includes\n\
<includes>\n\
#include <TBS/Services/DBus/DBusServicesImpl.h>\n\
\n\
\n\
\n\
<namespaceStart>\
	   Client::Client(TBS::Services::ICommChannelHolder::Ptr ch) : \n\
	       ch(ch), comChannelHolder(new TBS::Services::CommunicationChannelHolder(ch)) {\n\
	   }\n\
	   Client::Client() : \n\
		   ch(TBS::Services::DBusCommChannelProvider::getDefaultCommChannel()), comChannelHolder(new TBS::Services::CommunicationChannelHolder(ch)) {\n\
	   }\n\
	   Client::~Client(){ \n\
	   }\n\
	   <clientMethods>\n\
	   \n\
	   \n\
	   Server::Server(TBS::Services::ICommChannelHolder::Ptr ch) : \n\
	   	   ch(ch){\n\
		   \n\
	    } \n\
		Server::Server() : \n\
	   	   ch(TBS::Services::DBusCommChannelProvider::getDefaultCommChannel()){\n\
		   \n\
	    } \n\
	    Server::~Server(){ \n\
	    }\n\
	    \n\
	   <serverMethods>\n\
<namespaceEnd>\n\
\n\
#endif \n\
"

#endif
