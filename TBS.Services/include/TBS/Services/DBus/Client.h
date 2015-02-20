/*
 * Client.h
 *
 *  Created on: Sep 30, 2013
 *      Author: root
 */

#ifndef _NO_DBUS

#ifndef CLIENT_H_
#define CLIENT_H_
#include <dbus-c++/connection.h>
#include <memory>
#include "ConnectionWrapper.h"
#include "dbus-c++/introspection.h"
#include "dbus-c++/object.h"

#include <iostream>

namespace TBS {
		namespace Services {

			class CommunicationChannelHolder {
				public:
					CommunicationChannelHolder(ICommChannelHolder::Ptr ch) : ch(ch){

					}
					~CommunicationChannelHolder(){

					}

					DBus::Connection & getConnection(){
						if (!conn.get()){
							conn = std::unique_ptr<DBus::Connection>(new DBus::Connection(ConnectionWrapper::SessionBus(ch.cast<DBusCommChannelHolder>()->dispatcher())));
						}
						return *conn;
					}

				private:
					ICommChannelHolder::Ptr ch;
					std::unique_ptr<DBus::Connection> conn;
			};

			template<class TProxy>
			class DBusClientImpl: public TProxy, public DBus::IntrospectableProxy, public DBus::ObjectProxy {
				public:
					DBusClientImpl(DBus::Connection & conn) :
							DBus::ObjectProxy(conn, TProxy::dbuspath(), TProxy::dbusname().c_str()) {

					}

					virtual ~DBusClientImpl() {

					}
			};



		}
}


#endif /* CLIENT_H_ */

#endif
