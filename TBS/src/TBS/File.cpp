/*
 * File.cpp
 *
 *  Created on: Nov 11, 2014
 *      Author: dron
 */

#include "TBS/File.h"

#include <string.h>

#include "TBS/Log.h"

#include "Poco/Exception.h"
#include <fstream>

#include <sstream>
#include <iostream>

namespace TBS {

	std::string FileHelper::cmdOutput(std::string cmd) {
#ifndef _WIN32
		char buf[1024];
		FILE *p = popen(cmd.c_str(), "r");
		if (p == NULL) {
			LWARNING("File")<< "SystemHelper: Command <" << cmd << "> could not be run: " << strerror(errno) << LE;
			return std::string();
		}
		std::string s;
		for (size_t count; (count = fread(buf, 1, sizeof(buf), p));) {
			s += std::string(buf, buf + count);
		}
		pclose(p);
		return s;
#else
		LERROR("File") << "cmd on win not implemented: " << cmd << LE;
		throw Poco::Exception("Not implemented on win");
#endif
	}

	void FileHelper::write(std::string file, std::string content) {
		std::ofstream f(file.c_str());
		f.exceptions();
		if (!f.good()) {
			throw Poco::Exception((std::string) "Cannot write to file " + file);
		}
		f << content;
		f.close();

	}
	std::string FileHelper::read(std::string file) {
		std::ifstream f(file.c_str());
		f.exceptions();
		if (!f.good()) {
			throw Poco::Exception((std::string) "Cannot read from file " + file);
		}
		std::ostringstream buf;
		buf << f.rdbuf();
		return buf.str();

	}

	void FileHelper::append(const std::string& inFile, const std::string& outFile) {
		std::ifstream ifs(inFile.c_str(), std::ios_base::binary);
		if (not ifs.good()) {
			throw std::runtime_error(std::string("append(inFile, outFile): input file ") + inFile + std::string(" could not be opened"));
		}
		std::ofstream ofs(outFile.c_str(), std::ios_base::binary | std::ios_base::app);

		std::vector<char> block;
		block.reserve(10*1024);
		while (ifs.good()) {
			ifs.read(block.data(), block.capacity());
			int read = ifs.gcount();
			if (read > static_cast<std::streamsize>(block.capacity())) {
				throw std::runtime_error("append(inFile, outFile): more data read than allocated");
			}
			ofs.write(block.data(), read);
		}

		ifs.close();
		ofs.close();
	}
}  // namespace TBS
