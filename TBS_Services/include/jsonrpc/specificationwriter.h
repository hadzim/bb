/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    specificationwriter.h
 * @date    30.04.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef SPECIFICATIONWRITER_H
#define SPECIFICATIONWRITER_H

#include "procedure.h"
#include "specification.h"

namespace jsonrpc {
    class SpecificationWriter
    {
        public:
            static Json::Value toJsonValue(Procedure::Map& procedures);
            static std::string toString(Procedure::Map& procedures);
            static void toFile(const std::string& filename, Procedure::Map& procedures);

        private:
            static Json::Value toJsonLiteral(jsontype_t type);
            static void procedureToJsonValue(Procedure::Ptr procedure, Json::Value& target);
    };
}

#endif // SPECIFICATIONWRITER_H
