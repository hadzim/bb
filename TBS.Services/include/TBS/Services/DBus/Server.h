/*
 * Server.h
 *
 *  Created on: Sep 29, 2013
 *      Author: root
 */

#ifndef _NO_DBUS

#ifndef HWSERVER_H_
#define HWSERVER_H_

#include "ConnectionWrapper.h"
#include "Poco/SharedPtr.h"
#include "dbus-c++/introspection.h"
#include "dbus-c++/object.h"
#include "TBS/Services/Services.h"
#include "TBS/Services/DBus/DBusCommChannelHolder.h"
#include <iostream>
namespace TBS {
		namespace Services {

			template<class TInterface, class TAdaptor>
			class DBusServerImpl: public IServer {
				private:
					class InternalImpl: public TAdaptor, public DBus::IntrospectableAdaptor, public DBus::ObjectAdaptor {
						public:
							typedef Poco::SharedPtr<InternalImpl> Ptr;
							InternalImpl(typename TInterface::Ptr impl, DBus::Connection &connection) :
								TAdaptor(impl), DBus::ObjectAdaptor(connection, TAdaptor::dbuspath()) {
							}
							virtual ~InternalImpl(){
								std::cout << "dbus InternalImpl deleted" << std::endl;
							}



					};

				public:

					DBusServerImpl(ICommChannelHolder::Ptr se, typename TInterface::Ptr impl) :
						connection(ConnectionWrapper::SessionBus(se.cast<DBusCommChannelHolder>()->dispatcher())) {

						connection.request_name(TAdaptor::dbusname().c_str());
						serverObj = new InternalImpl(impl, connection);
					}
					virtual ~DBusServerImpl() {
						try {
							std::cout << "dbus DBusServerImpl deleted0" << std::endl;
							connection.disconnect();
							std::cout << "dbus DBusServerImpl deleted1" << std::endl;
							serverObj = NULL;
							std::cout << "dbus DBusServerImpl deleted2" << std::endl;
						} catch (::DBus::Error & e){
							std::cout << "dbus DBusServerImpl exc: " << e.message() << std::endl;
						}

					}

				private:
					DBus::Connection connection;
					typename InternalImpl::Ptr serverObj;
			};



		} /* namespace Services */
} /* namespace TBS */
#endif /* SERVER_H_ */

#endif
