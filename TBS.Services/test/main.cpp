/*
 * main.cpp
 *
 *  Created on: Oct 3, 2013
 *      Author: root
 */

#include "json/value.h"

#include <stdio.h>
#include <string>
#include <iostream>
#include "jsonrpc/rpc.h"
#include "jsonrpc/serverinterface.h"
#include "jsonrpc/connectors/httpinterfaceserver.h"
#include "jsonrpc/connectors/httpinterfaceclient.h"

#include "HWIO.h"
#include "Server/HWIO_Json.h"
#include "Client/HWIO_Json.h"
#include "HWIOSvc_Json.h"
#include "HWIOSvc_DBus.h"
#include <cstdarg>
#include "dbus-c++/debug.h"

using namespace jsonrpc;
using namespace std;

class AvImpl : public TBS::TSeries::Services::HWIO::IServices {
	virtual std::vector<std::string> Available(){
		std::vector<std::string> av;
		av.push_back("some");
		av.push_back("values");
		return av;
	}
	virtual void Reload(){

	}
};




/*
class SampleInterface: public AbstractServerInterface <SampleInterface> {
	public:
	SampleInterface(ServiceHandlers & handlers) :
			AbstractServerInterface<SampleInterface>("sample", handlers) {
			this->bindAndAddMethod(new Procedure("sayHello", PARAMS_BY_NAME, JSON_STRING, "name", JSON_STRING, NULL), &SampleInterface::sayHello);
			this->bindAndAddNotification(new Procedure("notifyServer", PARAMS_BY_NAME, NULL), &SampleInterface::notifyServer);
		}

		//methodssl_cert.pem
		void sayHello(const Json::Value& request, Json::Value& response) {
			response = "Hello: " + request["name"].asString();
		}

		//notification
		void notifyServer(const Json::Value& request) {
			cout << "server received some Notification" << endl;
		}

};

class SampleServer: public AbstractServer<SampleServer> {
	public:
		SampleServer() :
				AbstractServer<SampleServer>(new HttpServer(80, true, "ssl_cert.pem")) {
			this->bindAndAddMethod(new Procedure("sayHello", PARAMS_BY_NAME, JSON_STRING, "name", JSON_STRING, NULL), &SampleServer::sayHello);
			this->bindAndAddNotification(new Procedure("notifyServer", PARAMS_BY_NAME, NULL), &SampleServer::notifyServer);
		}

		//methodssl_cert.pem
		void sayHello(const Json::Value& request, Json::Value& response) {
			response = "Hello: " + request["name"].asString();
		}

		//notification
		void notifyServer(const Json::Value& request) {
			cout << "server received some Notification" << endl;
		}

};*/


int dinterfaceservermain() {
	try {
		TBS::TSeries::Services::HWIO::DBus::Server server;
		{
			AvImpl::Ptr p = new AvImpl();
			TBS::Services::IServer::Ptr s = server.createServices(new AvImpl());

			std::cout << "Server started successfully" << std::endl;
			getchar();


		}
	} catch (jsonrpc::JsonRpcException& e) {
		cerr << e.what() << endl;
	}
	return 0;
}


int interfaceservermain() {
	try {
		TBS::TSeries::Services::HWIO::Json::Server server(TBS::Services::JsonServerChannel(8989));
		{
			AvImpl::Ptr p = new AvImpl();
			TBS::Services::IServer::Ptr s = server.createServices(new AvImpl());
			server.start();
			std::cout << "Server started successfully" << std::endl;
			getchar();
			server.stop();

		}
	} catch (jsonrpc::JsonRpcException& e) {
		cerr << e.what() << endl;
	}
	return 0;
}

int interfaceclientmain() {

	TBS::Services::JsonClientChannel clientParams("localhost", 8989);
	TBS::TSeries::Services::HWIO::Json::Client client(clientParams);


//	std::cout << "connected" << std::endl;

	try {
		std::cout << "availabel: " << std::endl;
		std::vector <std::string> vals = client.Services().Available();
		for (int i = 0; i < vals.size(); i++){
			cout << vals[i] << endl;
		}
		//cout << c.CallMethod("sayHello", params) << endl;
	} catch (JsonRpcException & e) {
		cerr << e.what() << endl;
	}
	return 0;
}

int dinterfaceclientmain() {

	//TBS::Services::DBus clientParams("localhost", 8989);
	TBS::TSeries::Services::HWIO::DBus::Client client;


//	std::cout << "connected" << std::endl;

	try {
		std::cout << "availabel: " << std::endl;
		std::vector <std::string> vals = client.Services().Available();
		for (int i = 0; i < vals.size(); i++){
			cout << vals[i] << endl;
		}
		//cout << c.CallMethod("sayHello", params) << endl;
	} catch (JsonRpcException & e) {
		cerr << e.what() << endl;
	}
	return 0;
}

static void hdbg(const char *format, ...)
{
//#ifdef DEBUG


  {
    va_list args;
    va_start(args, format);

    fprintf(stderr, "dbus-c++: ");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");

    va_end(args);
  }

//#endif//DEBUG
}


int main(int argc, char **argv) {

	DBus::debug_log = hdbg;

	try {
		if (argc == 1) {
			dinterfaceservermain();
		} else {
			dinterfaceclientmain();
		}
	} catch (Poco::Exception & e) {
		std::cerr << "ERR: " << e.displayText() << std::endl;
	}
	return 0;
}
