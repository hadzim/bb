/*
 * RWa.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: dron
 */

#include <BB/RW.h>
#include <opencv2/highgui/highgui.hpp>
#include <Poco/Base64Encoder.h>
#include <Poco/Base64Decoder.h>
#include "json/reader.h"
#include "json/writer.h"
#include "json/value.h"
#include <fstream>
#include <Poco/String.h>
#include <Poco/File.h>

#include "Poco/Exception.h"

namespace BB {

	std::string RW::toBase64(const std::string &source) {
		std::istringstream in(source);
		std::ostringstream out;
		Poco::Base64Encoder b64out(out);
		b64out.rdbuf()->setLineLength(0);

		std::copy(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>(),
				std::ostreambuf_iterator<char>(b64out));
		b64out.close(); // always call this at the end!

		return out.str();
	}

	std::string RW::fromBase64(const std::string &source) {
		std::istringstream in(source);
		std::ostringstream out;
		Poco::Base64Decoder b64in(in);

		std::copy(std::istreambuf_iterator<char>(b64in), std::istreambuf_iterator<char>(),
				std::ostreambuf_iterator<char>(out));

		return out.str();
	}

	static std::string prefix = "data:image/jpg;base64,";

	static std::vector<unsigned char> mat2Bytes(const cv::Mat & src) {

		std::vector<unsigned char> outVet;
		std::vector <int> params;
		params.push_back(CV_IMWRITE_JPEG_QUALITY);
		params.push_back(70);
		imencode(".jpg", src, outVet, params);

		//LERROR("SIZE") << "Before:" << (src.rows * src.cols) << " AFTER: " << outVet.size() << LE;

		return outVet;
	}

	std::string dumpStr(std::string s) {
		std::stringstream ss;
		if (s.size() > 10) {
			for (int i = 0; i < 10; i++) {
				ss << s.at(i);
			}
			ss << " ... ";

			for (int i = s.size() - 10; i < s.size(); i++) {
				ss << s.at(i);
			}
			ss << " (" << s.size() << ")" << std::endl;
		}
		return ss.str();
	}

	std::string RW::image2string(const cv::Mat & mat) {
		auto bytes = mat2Bytes(mat);

		std::stringstream s;
		s << prefix << RW::toBase64(std::string((char *) bytes.data(), bytes.size()));

		return s.str();

	}
	cv::Mat RW::string2image(const std::string & str) {

		std::string img = str.substr(prefix.size());

		std::string bin = RW::fromBase64(img);

		std::vector<unsigned char> data;
		for (int i = 0; i < bin.size(); i++) {
			data.push_back(bin.at(i));
		}

		return cv::imdecode(data, CV_LOAD_IMAGE_COLOR);
	}

std::string RW::json2String(const Json::Value & v) {
	Json::FastWriter fw;
	return fw.write(v);
}
std::string RW::json2OneLine(const Json::Value & v) {
	std::string s = json2String(v);
	return Poco::replace(s, "\n", "");
}
Json::Value RW::string2json(const std::string & s) {
	Json::Reader r;
	Json::Value v;
	r.parse(s, v);
	return v;
}

Json::Value RW::file2json(std::string file) {
	if (!Poco::File(file).exists()) {
		throw Poco::Exception(std::string("File does not exist: ") + file);
	}
	Json::Reader r;
	Json::Value v;
	std::ifstream fileStream(file);
	r.parse(fileStream, v);
	return v;
}
void RW::json2file(std::string file, const Json::Value & v) {
	std::ofstream f(file);
	Json::StyledStreamWriter fw;
	fw.write(f, v);
}

} /* namespace BB */
