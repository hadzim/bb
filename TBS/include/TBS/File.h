/*
 * File.h
 *
 *  Created on: Nov 11, 2014
 *      Author: dron
 */

#ifndef TBSFILE_H_
#define TBSFILE_H_
#include <string>
#include "TBS/TBS.h"

namespace TBS {

	class TBS_API FileHelper {
		public:
			static void write(std::string file, std::string content);
			static std::string read(std::string file);
			static std::string cmdOutput(std::string cmd);
			static void append(const std::string& inFile, const std::string& outFile);
	};

}  // namespace TBS

#endif /* FILE_H_ */
