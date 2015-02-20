/*
 * ServiceServer.cpp
 *
 *  Created on: Oct 3, 2013
 *      Author: root
 */

#include "jsonrpc/ServiceHandlers.h"

#include "TBS/Log.h"

namespace jsonrpc {

	ServiceHandlers::ServiceHandlers() {
		//std::cout << "create handlers " << this << std::endl;
		LDEBUG("DBG") << (uintptr_t)this << " construct "  << LE;
	}

	ServiceHandlers::~ServiceHandlers() {
		TBS::dumpBacktrace("DBG", "DBG", 5);
		//std::cout << "delete handlers " << this << std::endl;
		LDEBUG("DBG") << (uintptr_t)this << " destruct "  << LE;
	}

	void ServiceHandlers::registerHandler(std::string name, RpcProtocolServer::Ptr service) {
		//std::cout << "register handler " << this << " " << name << std::endl;
		this->handlers[name] = service;
	}
	void ServiceHandlers::unregisterHandler(std::string name) {
		std::cout << "unregister handler " << this << " " << name << std::endl;
		this->handlers.erase(name);
	}

	RpcProtocolServer::Ptr ServiceHandlers::getHandler(std::string name) {
		//LDEBUG("DBG") << (uintptr_t)this <<  " getHandler "  << LE;

		//std::cout << "debug  handlers " << std::endl;
		//for (auto h : this->handlers){
			//std::cout << "already in handler " << h.first << std::endl;
		//}

		//std::cout << "get handler " << this << " " << name << std::endl;
		if (this->handlers.find(name) != this->handlers.end()) {
			//std::cout << "register handler " << this << " " << name << " ok" << std::endl;
			return this->handlers[name];
		}
		throw Poco::Exception("Unknown service: " + name);
	}

} /* namespace jsonrpc */
