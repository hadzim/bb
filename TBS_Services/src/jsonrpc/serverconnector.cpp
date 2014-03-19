/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    serverconnector.cpp
 * @date    31.12.2012
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "jsonrpc/serverconnector.h"
#include "jsonrpc/specificationwriter.h"
#include <cstdlib>

using namespace std;

namespace jsonrpc
{

    SingleServerConnector::SingleServerConnector()
    {
        this->handler = NULL;
    }

    SingleServerConnector::~SingleServerConnector()
    {
    }
    /*
    bool SingleServerConnector::OnRequest(const std::string& request, void* addInfo)
    {
        string response;
        if (this->handler != NULL)
        {
            this->handler->HandleRequest(request, response);
            this->SendResponse(response, addInfo);
            return true;
        }
        else
        {
            return false;
        }
    }
    */

    string SingleServerConnector::GetSpecification()
    {
        return SpecificationWriter::toString(this->handler->GetProcedures());
    }

    void SingleServerConnector::SetHandler(RpcProtocolServer& handler)
    {
        this->handler = &handler;
    }

    RpcProtocolServer & SingleServerConnector::GetHandler(){
    	return *this->handler;
    }

} /* namespace jsonrpc */
