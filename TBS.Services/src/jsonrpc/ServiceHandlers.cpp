/*
 * ServiceServer.cpp
 *
 *  Created on: Oct 3, 2013
 *      Author: root
 */

#include "jsonrpc/ServiceHandlers.h"

namespace jsonrpc {

	ServiceHandlers::ServiceHandlers() {
		//std::cout << "create handlers " << this << std::endl;
	}

	ServiceHandlers::~ServiceHandlers() {
		//std::cout << "delete handlers " << this << std::endl;
	}

	void ServiceHandlers::registerHandler(std::string name, RpcProtocolServer::Ptr service) {
		//std::cout << "register handler " << this << " " << name << std::endl;
		this->handlers[name] = service;
	}
	void ServiceHandlers::unregisterHandler(std::string name) {
		//std::cout << "unregister handler " << this << " " << name << std::endl;
		this->handlers.erase(name);
	}

	RpcProtocolServer::Ptr ServiceHandlers::getHandler(std::string name) {
		//std::cout << "get handler " << this << " " << name << std::endl;
		if (this->handlers.find(name) != this->handlers.end()) {
			//std::cout << "register handler " << this << " " << name << " ok" << std::endl;
			return this->handlers[name];
		}
		throw Poco::Exception("Unknown service");
	}

} /* namespace jsonrpc */
