/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    specificationparser.h
 * @date    12.03.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef SPECIFICATIONPARSER_H
#define SPECIFICATIONPARSER_H

#include "procedure.h"
#include "exception.h"

namespace jsonrpc {


    class SpecificationParser
    {
        public:
            static Procedure::MapPtr GetProceduresFromFile(const std::string& filename);
            static Procedure::MapPtr GetProceduresFromString(const std::string& spec);


        private:
            static Procedure::Ptr GetProcedure(Json::Value& val);
            static void GetFileContent(const std::string& filename, std::string& target);
            static jsontype_t toJsonType(Json::Value& val);
    };
}
#endif // SPECIFICATIONPARSER_H
