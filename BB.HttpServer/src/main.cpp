/*
 * main.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */



#include "BB/App.h"
#include "BB/ForwardApp.h"
#include "BB/Forwarder/HttpServer/HttpServerForwarder.h"

namespace BB {
	class Factory : public IForwarderFactory {
		virtual IForwarder::Ptr createForwarder(){
			return new HttpServerForwarder(8111);
		}
	};
}

FWD_BB_MAIN(BB::Factory)


