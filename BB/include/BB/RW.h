/*
 * RWa.h
 *
 *  Created on: Nov 17, 2015
 *      Author: dron
 */

#ifndef RWA_H_
#define RWA_H_
#include "json/value.h"
#include <string>

#include "opencv2/core/core.hpp"

namespace BB {

class RW {
	public:
		static std::string json2String(const Json::Value & v);
		static std::string json2OneLine(const Json::Value & v);
		static Json::Value string2json(const std::string & v);

		static Json::Value file2json(std::string file);
		static void json2file(std::string file, const Json::Value & v);

		static std::string toBase64(const std::string &source);
		static std::string fromBase64(const std::string &source);

		static std::string image2string(const cv::Mat & mat);
		static cv::Mat string2image(const std::string & str);
};


} /* namespace BB */

#endif /* RWA_H_ */
