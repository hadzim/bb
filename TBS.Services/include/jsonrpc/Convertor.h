/*
 * Convertor.h
 *
 *  Created on: Oct 7, 2013
 *      Author: root
 */

#ifndef CONVERTOR_H_
#define CONVERTOR_H_

#include <vector>
#include <string>
#include "json/value.h"
#include <stdexcept>
#include "TBS/Nullable.h"

namespace jsonrpc {

#define CPP2JSONBODY Json::Value v(val); return v;




	template<typename T> class InternalConvertor;

	class Convertor {
		public:
			template<typename T>
			static T json2Cpp(const Json::Value & val)
			{
			   return InternalConvertor<T>::json2Cpp(val);
			}
			template<typename T>
			static Json::Value cpp2Json(const T & val)
			{
			   return InternalConvertor<T>::cpp2Json(val);
			}
	};


	// class template:
	template<class T>
	class InternalConvertor {
		public:
			static T json2Cpp(const Json::Value & val) {
				return val;
			}
			static Json::Value cpp2Json(const T & val) {
				CPP2JSONBODY
			}
	};

	// class template:
	template<>
	class InternalConvertor<int> {
		public:
			static int json2Cpp(const Json::Value & val) {
				return static_cast<int>(val.asInt());
			}
			static Json::Value cpp2Json(const int & val) {
				Json::Value v((Json::Int)val); return v;
			}
	};


	// class template:
	template<>
	class InternalConvertor<Poco::Int64> {
		public:
			static Poco::Int64 json2Cpp(const Json::Value & val) {
				return val.asInt();
			}
			static Json::Value cpp2Json(const Poco::Int64 & val) {
				CPP2JSONBODY
			}
	};

	// class template:
	template<>
	class InternalConvertor<bool> {
		public:
			static bool json2Cpp(const Json::Value & val) {
				return val.asBool();
			}
			static Json::Value cpp2Json(const bool & val) {
				CPP2JSONBODY
			}
	};

	// class template:
		template<>
		class InternalConvertor<double> {
			public:
				static double json2Cpp(const Json::Value & val) {
					return val.asDouble();
				}
				static Json::Value cpp2Json(const double & val) {
					CPP2JSONBODY
				}
		};


	// class template:
	template<>
	class InternalConvertor<std::string> {
		public:
			static std::string json2Cpp(const Json::Value & val) {
				return val.asString();
			}
			static Json::Value cpp2Json(const std::string & val) {
				CPP2JSONBODY
			}
	};


	// class template:
	template <typename IntT>
	class InternalConvertor<std::vector<IntT> > {
		public:
			static std::vector<IntT>  json2Cpp(const Json::Value & val) {
				std::vector<IntT> vals;
				if (val.isArray()){
					for (unsigned int i = 0; i < val.size(); i++){
						vals.push_back(Convertor::json2Cpp<IntT>(val[i]));
					}
				} else {
					throw std::runtime_error("given value is not an array");
				}
				return vals;
			}
			static Json::Value cpp2Json(const std::vector<IntT> & val) {
				Json::Value retval(Json::arrayValue);
				for (typename std::vector<IntT>::const_iterator i = val.begin(); i != val.end(); i++){
					retval.append(Convertor::cpp2Json<IntT>(*i));
				}
				return retval;
			}
	};

	// class template:
	template <typename IntT>
	class InternalConvertor<TBS::Nullable<IntT> > {
		public:
			static TBS::Nullable<IntT>  json2Cpp(const Json::Value & val) {
				TBS::Nullable<IntT> rval;
				if (val.isNull()){
					return rval;
				}
				if (val.isString() && val.asString() == "null"){
					return rval;
				}
				rval.set(Convertor::json2Cpp<IntT>(val));
				return rval;
			}
			static Json::Value cpp2Json(const TBS::Nullable<IntT> & val) {
				if (val.isEmpty()){
					return Json::Value();
				}
				return Convertor::cpp2Json<IntT>(val.cref());
			}
	};

} /* namespace jsonrpc */
#endif /* CONVERTOR_H_ */
