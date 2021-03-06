#ifndef CLIENTTEMPLATE_CLIENT_H_
#define CLIENTTEMPLATE_CLIENT_H_

#define TEMPLATE_STUB "\
/**\n\
 * THIS FILE IS GENERATED BY genTool, DO NOT CHANGE IT!!!!!\n\
 */\n\
#ifndef _<STUBNAME>_SERVICE_JSON_H_\n\
#define _<STUBNAME>_SERVICE_JSON_H_\n\
\n\
#include <Poco/SharedPtr.h>\n\
#include <Poco/BasicEvent.h>\n\
#include <string>\n\
#include <vector>\n\
#include <map>\n\
#include <memory>\n\
#include <TBS/Nullable.h>\n\
#include <TBS/Services/Services.h>\n\
#include <TBS/Services/Json/JsonServices.h>\n\
#include <TBS/Services/Serial/SerialServices.h>\n\
///includes\n\
<includes>\n\
\n\
\n\
<namespaceStart>\
       class GEN_SERVICE_API Client {\n\
			public:\n\
				typedef Poco::SharedPtr <Client> Ptr;\n\
				\n\
				Client(const TBS::Services::JsonClientParams & ch);\n\
				Client(const TBS::Services::SerialParams & ch);\n\
				~Client();\n\
				\n\
				\n //methods \n\
				<clients>\n\
				\n\
		private: \n\
				TBS::Nullable<TBS::Services::JsonClientParams> httpCh;\n\
				TBS::Nullable<TBS::Services::SerialParams> serialCh;\n\
				<private>\n\
			};\n\
			\n\
			\n\
		class GEN_SERVICE_API Server {\n\
			public:\n\
				typedef Poco::SharedPtr<Server> Ptr;\n\
				\n\
				static Server::Ptr createJsonServer(const TBS::Services::JsonServerParams & p);\n\
				static Server::Ptr createJsonpServer(const TBS::Services::JsonServerParams & p);\n\
				static Server::Ptr createWsServer(const TBS::Services::JsonServerParams & p);\n\
				static Server::Ptr createRawServer(const TBS::Services::JsonServerParams & p);\n\
				static Server::Ptr createSerialServer(const TBS::Services::SerialParams & p);\n\
				\n\
				~Server();\n\
				\n\
				void start();\n\
				void stop();\n\
			public:\n\
				<servers>\n\
				\n\
			private:\n\
				Server(TBS::Services::ICommChannelHolder::Ptr ch);\n\
				TBS::Services::ICommChannelHolder::Ptr channel;\n\
		};\n\
<namespaceEnd>\n\
#endif //_<STUBNAME>_SERVICE_H_\n\
"


#define TEMPLATE_CPP_STUB "\
/**\n\
 * THIS FILE IS GENERATED BY genTool, DO NOT CHANGE IT!!!!!\n\
 */\n\
///includes\n\
<includes>\n\
#include <TBS/Services/Json/JsonServicesImpl.h>\n\
\n\
//convertors\n\
<convertors>\n\
\n\
<namespaceStart>\
	   Client::Client(const TBS::Services::JsonClientParams & ch){\n\
			httpCh.set(ch);\n\
	   }\n\
	   Client::Client(const TBS::Services::SerialParams & ch){\n\
			serialCh.set(ch);\n\
	   }\n\
	   Client::~Client(){}\n\
	   \n\
	   <clientMethods>\n\
	   \n\
	   \n\
	   \n\
	   Server::Ptr Server::createJsonServer(const TBS::Services::JsonServerParams & p){\n\
		   return new Server(new TBS::Services::JsonCommChannelHolder(p));\n\
	   }\n\
	   Server::Ptr Server::createJsonpServer(const TBS::Services::JsonServerParams & p){\n\
		   return new Server(new TBS::Services::JsonpCommChannelHolder(p));\n\
	   }\n\
	   Server::Ptr Server::createWsServer(const TBS::Services::JsonServerParams & p){\n\
		   return new Server(new TBS::Services::WsCommChannelHolder(p));\n\
	   }\n\
	   Server::Ptr Server::createRawServer(const TBS::Services::JsonServerParams & p){\n\
		   return new Server(new TBS::Services::RawCommChannelHolder(p));\n\
	   }\n\
	   Server::Ptr Server::createSerialServer(const TBS::Services::SerialParams & p){\n\
		   return new Server(new TBS::Services::SerialCommChannelHolder(p));\n\
	   }\n\
	   \n\
	   \n\
	   Server::Server(TBS::Services::ICommChannelHolder::Ptr ch) : \n\
	   	   channel(ch){\n\
		   \n\
	    } \n\
	    Server::~Server(){}\n\
	    void Server::start(){ \n\
	   	   channel.cast<TBS::Services::AJsonCommChannelHolder>()->getInterface().StartListening();\n\
	    } \n\
	    void Server::stop(){ \n\
	   	   channel.cast<TBS::Services::AJsonCommChannelHolder>()->getInterface().StopListening();\n\
	    } \n\
	    \n\
	   <serverMethods>\n\
<namespaceEnd>\n\
\n\
"

#endif
