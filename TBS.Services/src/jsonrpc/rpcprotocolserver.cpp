/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    rpcprotocolserver.cpp
 * @date    31.12.2012
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "jsonrpc/rpcprotocolserver.h"
#include "jsonrpc/errors.h"
#include "jsonrpc/server.h"

#include <iostream>

using namespace std;

namespace jsonrpc
{
    RpcProtocolServer::RpcProtocolServer(AbstractRequestHandler::RawPtr server, Procedure::MapPtr procedures, AbstractAuthenticator::Ptr auth) :
        procedures(procedures),
        authManager(auth),
        server(server)
    {
    }

    RpcProtocolServer::RpcProtocolServer(AbstractRequestHandler::RawPtr server, AbstractAuthenticator::Ptr auth) :
        procedures(new Procedure::Map()),
        authManager(auth),
        server(server)
    {
    }

    RpcProtocolServer::~RpcProtocolServer()
    {

    }

    void RpcProtocolServer::HandleRequest(const std::string& request,
                                          std::string& retValue, bool fixParams)
    {

        Json::Reader reader;
        Json::Value req;
        Json::Value response, resp;
        Json::FastWriter w;

        std::string errorMessage;

        try {
			if (reader.parse(request, req, false))
			{
				//It could be a Batch Request
				if (req.isArray())
				{
					this->HandleBatchRequest(req, response);
				} //It could be a simple Request
				else if (req.isObject())
				{
					this->HandleSingleRequest(req, response, fixParams);
				}
			}
			else
			{
				response = Errors::GetErrorBlock(Json::nullValue, Errors::ERROR_RPC_JSON_PARSE_ERROR);
			}
			retValue = w.write(response);

    	} catch (Poco::Exception & e){
			errorMessage = e.message();
			std::cout << "EXception: " << e.displayText() << std::endl;
		} catch (std::exception & e){
			std::cout << "EXception: " << e.what() << std::endl;
			errorMessage = e.what();
		} catch (...){
			std::cout << "EXception: ??" << std::endl;
			errorMessage = "Unknown error";
		}

		if (!errorMessage.empty()){
			response = Errors::GetCustomErrorBlock(req, -30000, errorMessage);
			retValue = w.write(response);
		}

    }

    void RpcProtocolServer::SetAuthenticator(AbstractAuthenticator::Ptr auth)
    {
        this->authManager = auth;
    }

    void RpcProtocolServer::HandleSingleRequest(Json::Value &req, Json::Value& response, bool fixParams)
    {
        int error = this->ValidateRequest(req, fixParams);
        if (error == 0)
        {
            try
            {
                this->ProcessRequest(req, response);
            }
            catch (const JsonRpcException & exc)
            {
                response = Errors::GetErrorBlock(req, exc);
            }
        }
        else
        {
            response = Errors::GetErrorBlock(req, error);
        }
    }

    void RpcProtocolServer::HandleBatchRequest(Json::Value &req, Json::Value& response)
    {
        for (unsigned int i = 0; i < req.size(); i++)
        {
            this->HandleSingleRequest(req, response[i], false);
        }
    }

    int RpcProtocolServer::ValidateRequest(Json::Value& request, bool fixParams)
    {
    	//std::cout << "validate request: " << std::endl;
        int error = 0;
        Procedure::Ptr proc;
        if (!(request.isMember(KEY_REQUEST_METHODNAME)
              && request.isMember(KEY_REQUEST_VERSION)
              && request.isMember(KEY_REQUEST_PARAMETERS)))
        {
            error = Errors::ERROR_RPC_INVALID_REQUEST;
        }
        else
        {
        	/*
        	for (Procedure::Map::iterator it = procedures->begin(); it != procedures->end(); it++){
        		std::cout << "registered: " << it->first << std::endl;
        	}
        	std::cout << "search for " << request[KEY_REQUEST_METHODNAME].asString() << std::endl;*/
           Procedure::Map::iterator it = procedures->find(request[KEY_REQUEST_METHODNAME].asString());
            if (it != this->procedures->end())
            {
                proc = (*this->procedures)[request[KEY_REQUEST_METHODNAME].asString()];
                if(request.isMember(KEY_REQUEST_ID) && proc->GetProcedureType() == RPC_NOTIFICATION)
                {
                    error = Errors::ERROR_SERVER_PROCEDURE_IS_NOTIFICATION;
                }
                else if(!request.isMember(KEY_REQUEST_ID) && proc->GetProcedureType() == RPC_METHOD)
                {
                    error = Errors::ERROR_SERVER_PROCEDURE_IS_METHOD;
                }
                else if (proc->ValdiateParameters(request[KEY_REQUEST_PARAMETERS], fixParams))
                {
                    if (!this->authManager.isNull())
                    {
                        error = this->authManager->CheckPermission(
                                    request[KEY_AUTHENTICATION],
                                    proc->GetProcedureName());
                    }
                }
                else
                {
                    error = Errors::ERROR_RPC_INVALID_PARAMS;
                }
            }
            else
            {
            	std::cout << "method not found" << std::endl;
                error = Errors::ERROR_RPC_METHOD_NOT_FOUND;
            }
        }
        return error;
    }

    void RpcProtocolServer::ProcessRequest(const Json::Value& request,
                                           Json::Value& response)
    {
        Procedure::Ptr method = (*this->procedures)[request[KEY_REQUEST_METHODNAME].asString()];
        Json::Value result;

        if (method->GetProcedureType() == RPC_METHOD)
        {

            server->handleMethodCall(method, request[KEY_REQUEST_PARAMETERS],
                                     result);

        	
            response[KEY_REQUEST_VERSION] = JSON_RPC_VERSION;
            response[KEY_RESPONSE_RESULT] = result;
            response[KEY_REQUEST_ID] = request[KEY_REQUEST_ID];
            if (!this->authManager.isNull())
            {
                this->authManager->ProcessAuthentication(
                            request[KEY_AUTHENTICATION],
                            response[KEY_AUTHENTICATION]);
            }
        }
        else
        {
            server->handleNotificationCall(method, request[KEY_REQUEST_PARAMETERS]);
            response = Json::Value::null;
        }
    }

    void RpcProtocolServer::AddProcedure(Procedure::Ptr procedure)
    {
        (*this->procedures)[procedure->GetProcedureName()] = procedure;
    }

    Procedure::Map & RpcProtocolServer::GetProcedures()
    {
        return *(this->procedures);
    }

} /* namespace jsonrpc */

