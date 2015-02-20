/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    httpserver.cpp
 * @date    31.12.2012
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "jsonrpc/connectors/SerialInterfaceServer.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/SecureServerSocket.h"
#include "Poco/Net/HTTPBasicCredentials.h"

#include "Poco/Net/HTTPServer.h"
#include <Poco/StreamCopier.h>
#include <Poco/Thread.h>
#include "jsonrpc/ServiceHandlers.h"

#include "Poco/MD5Engine.h"
#include "Poco/DigestStream.h"
#include "Poco/DigestEngine.h"

#include "Poco/File.h"
#include <fstream>

#include "HttpServerHelpers.h"
#include "TBS/Log.h"

#include "serial/serial.h"

#include "Poco/Delegate.h"

#include "SerialLayerHelpers.h"

namespace jsonrpc {


	SerialInterfaceServer::SerialInterfaceServer(const TBS::Services::SerialParams & p) :
			p(p), listening(false), stopped(false) {

		layer = new TBS::Services::SerialTransportLayer(p.getSerialLayer());
		server = TBS::Services::createServer(layer, p.getTransportLayer());

		t.start(*this);

	}

	SerialInterfaceServer::~SerialInterfaceServer() {
		stopped = true;
		t.join();
	}

	bool SerialInterfaceServer::StartListening() {
		listening = true;
		return true;
	}

	bool SerialInterfaceServer::StopListening() {
		listening = false;
		return true;
	}

	void SerialInterfaceServer::onRequest(TBS::Services::ITransportServer::Message & msg){
		this->handlers()->getHandler(msg.interface)->HandleRequest(msg.request, msg.response);
	}


	void SerialInterfaceServer::run(){
		TBS::threadDebug();
		try {

			server->OnMessage += Poco::delegate(this, &SerialInterfaceServer::onRequest);
			LTRACE("json") << "bg thread " << LE;

			while (!stopped){

				server->loop();

				Poco::Thread::sleep(p.getSerialLayer().getSleepTimeMs());
			}

			server->OnMessage -= Poco::delegate(this, &SerialInterfaceServer::onRequest);

		} catch (Poco::Exception & e){
			LERROR("json") << "bg thread failed: " << e.displayText() << LE;

		} catch (std::exception & e){
			LERROR("json") << "bg thread failed: " << e.what() << LE;
		} catch (...){
			LERROR("json") << "bg thread failed: ??" << LE;
		}
	}

} /* namespace jsonrpc */
