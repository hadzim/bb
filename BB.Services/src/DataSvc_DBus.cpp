#ifndef _NO_DBUS 
/**
 * THIS FILE IS GENERATED BY genTool, DO NOT CHANGE IT!!!!!
 */
///includes
#include "BB/Services//DataSvc_DBus.h"
#include "Client/Data_DBus.h"
#include "Server/Data_DBus.h"

#include <TBS/Services/DBus/DBusServicesImpl.h>



namespace TBS { 
	namespace BB { 
		namespace Services { 
			namespace Data { 
				namespace DBus { 
	   Client::Client(TBS::Services::ICommChannelHolder::Ptr ch) : 
	       ch(ch), comChannelHolder(new TBS::Services::CommunicationChannelHolder(ch)) {
	   }
	   Client::Client() : 
		   ch(TBS::Services::DBusCommChannelProvider::getDefaultCommChannel()), comChannelHolder(new TBS::Services::CommunicationChannelHolder(ch)) {
	   }
	   Client::~Client(){ 
	   }
	   TBS::BB::Services::Data::IDataCollector & Client::DataCollector(){
   if (!datacollector_) {      datacollector_ =  new TBS::Services::DBusClientImpl<TBS::BB::Services::Data::Stub::DataCollector_DBusClient>(comChannelHolder->getConnection());
   }   return *datacollector_;
}
TBS::BB::Services::Data::IDataDistributor & Client::DataDistributor(){
   if (!datadistributor_) {      datadistributor_ =  new TBS::Services::DBusClientImpl<TBS::BB::Services::Data::Stub::DataDistributor_DBusClient>(comChannelHolder->getConnection());
   }   return *datadistributor_;
}

	   
	   
	   Server::Server(TBS::Services::ICommChannelHolder::Ptr ch) : 
	   	   ch(ch){
		   
	    } 
		Server::Server() : 
	   	   ch(TBS::Services::DBusCommChannelProvider::getDefaultCommChannel()){
		   
	    } 
	    Server::~Server(){ 
	    }
	    
	   TBS::Services::IServer::Ptr Server::createDataCollector(TBS::BB::Services::Data::IDataCollector::Ptr impl){
   return new TBS::Services::DBusServerImpl<TBS::BB::Services::Data::IDataCollector, TBS::BB::Services::Data::Stub::DataCollector_DBusServer>(ch, impl);
}
TBS::Services::IServer::Ptr Server::createDataDistributor(TBS::BB::Services::Data::IDataDistributor::Ptr impl){
   return new TBS::Services::DBusServerImpl<TBS::BB::Services::Data::IDataDistributor, TBS::BB::Services::Data::Stub::DataDistributor_DBusServer>(ch, impl);
}

 } 
 } 
 } 
 } 
 } 


#endif 

