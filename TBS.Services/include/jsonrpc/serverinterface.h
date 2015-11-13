/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    server.h
 * @date    30.12.2012
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef SERVERINT_H_
#define SERVERINT_H_

#include <map>
#include <string>
#include <vector>

#include "rpcprotocolserver.h"
#include "ServiceHandlers.h"

namespace jsonrpc
{
    template<class S>
    class AbstractServerInterface : public AbstractRequestHandler
    {
        public:
            typedef void(S::*methodPointer_t)(const Json::Value &parameter, Json::Value &result);
            typedef void(S::*notificationPointer_t)(const Json::Value &parameter);

            AbstractServerInterface(std::string name, ServiceHandlers::Ptr handlers) :
                name(name),
                handlers(handlers)
            {
            	this->handler = new RpcProtocolServer(this);
                handlers->registerHandler(name, handler);
            }

            virtual ~AbstractServerInterface()
            {
                handlers->unregisterHandler(name);
            }


            virtual void handleMethodCall(Procedure::Ptr proc, const Json::Value& input, Json::Value& output)
            {
                S* instance = dynamic_cast<S*>(this);
                (instance->*methods[proc->GetProcedureName()])(input, output);
            }

            virtual void handleNotificationCall(Procedure::Ptr proc, const Json::Value& input)
            {
                S* instance = dynamic_cast<S*>(this);
                (instance->*notifications[proc->GetProcedureName()])(input);
            }

        protected:
            virtual bool bindMethod(std::string& name, methodPointer_t method)
            {
                if(this->handler->GetProcedures().find(name) != this->handler->GetProcedures().end() && this->handler->GetProcedures()[name]->GetProcedureType() == RPC_METHOD)
                {
                    this->methods[name] = method;
                    return true;
                }
                return false;
            }

            virtual bool bindNotification(std::string& name, notificationPointer_t notification)
            {
                if(this->handler->GetProcedures().find(name) != this->handler->GetProcedures().end() && this->handler->GetProcedures()[name]->GetProcedureType() == RPC_NOTIFICATION)
                {
                    this->notifications[name] = notification;
                    return true;
                }
                return false;
            }

            virtual bool bindAndAddMethod(Procedure* proc, methodPointer_t pointer)
            {
                if(proc->GetProcedureType() == RPC_METHOD)
                {
                    this->handler->AddProcedure(proc);
                    this->methods[proc->GetProcedureName()] = pointer;
                    return true;
                }
                return false;
            }

            virtual bool bindAndAddNotification(Procedure::Ptr proc, notificationPointer_t pointer)
            {
                if(proc->GetProcedureType() == RPC_NOTIFICATION)
                {
                    this->handler->AddProcedure(proc);
                    this->notifications[proc->GetProcedureName()] = pointer;
                    return true;
                }
                return false;
            }

        private:
            std::string name;
            ServiceHandlers::Ptr handlers;
            RpcProtocolServer::Ptr handler;
            std::map<std::string, methodPointer_t> methods;
            std::map<std::string, notificationPointer_t> notifications;
    };

} /* namespace jsonrpc */
#endif /* SERVER_H_ */
