/*
 * ThreadStatus.h
 *
 *  Created on: Jul 24, 2013
 *      Author: root
 */

#ifndef THREADSTATUS_H_
#define THREADSTATUS_H_

#include <map>
#include <string>
#include <vector>

#include <Poco/SharedMemory.h>
#include "Poco/Mutex.h"
#include "Poco/NamedMutex.h"

#include <typeinfo>
#include "TBS/TBS.h"

#define THREAD_DUMP(message) TBS::DiagnosticsMemory::instance().writer().writeThread(message);
#define CLASS_DUMP(obj, message) TBS::DiagnosticsMemory::instance().writer().writeStatus(typeid(obj).name(), message);

namespace TBS {

	typedef int LWP;

	std::string trimString(std::string s);

	class TBS_API DiagnosticsStatusMemory {
		public:

			static const unsigned int MaxCount = 128;
			static const unsigned int MaxStringLength = 128;

			static int size();

			DiagnosticsStatusMemory(char * start);
			char * keyData(int threadNumber);
			char * stringData(int threadNumber);

			int records();
			char * cntPointer();

			static std::string data(char * dataPtr);

			static Poco::NamedMutex m;

		private:
			char * start;
	};

	typedef std::pair<std::string, std::string> DiagnosticStatus;
	typedef std::vector<DiagnosticStatus> Diagnostics;

	class TBS_API DiagnosticsStatusReader {
		public:
			DiagnosticsStatusReader(char * start);
			Diagnostics getDiagnosticsInfo();
		private:
			DiagnosticsStatusMemory mem;
	};

	class TBS_API DiagnosticsStatusWriter {
		public:

			DiagnosticsStatusWriter(char * start);
			void clear();
			void writeStatus(std::string key, std::string status);
			void writeThread(std::string status);
			void rebuildIndexes();
		private:

			typedef std::map<std::string, int> Indexes;
			Indexes indexes;

			DiagnosticsStatusMemory mem;

	};

	class TBS_API DiagnosticsMemory {
		public:
			static DiagnosticsMemory & instance();

			DiagnosticsStatusWriter & writer();
			DiagnosticsStatusReader & reader();

		private:
			DiagnosticsMemory();
			~DiagnosticsMemory();
		private:
			Poco::SharedMemory mem;
			static Poco::Mutex m;

			char * buffer;

			DiagnosticsStatusWriter * writer_;
			DiagnosticsStatusReader * reader_;
	};

} /* namespace TBS */
#endif /* THREADSTATUS_H_ */
