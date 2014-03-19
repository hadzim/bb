/*
 * ThreadStatus.cpp
 *
 *  Created on: Jul 24, 2013
 *      Author: root
 */

#include "TBS/ThreadStatus.h"

#include <stdio.h>
#include <string.h>

#ifndef __WIN32__
#include <unistd.h>
#include <sys/syscall.h>

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

#endif

#include <Poco/String.h>

#include <iostream>
#include <Poco/Thread.h>
#include <fstream>
#include <Poco/File.h>
#include <Poco/NumberFormatter.h>
#include "TBS/Log.h"

namespace TBS {

	Poco::NamedMutex DiagnosticsStatusMemory::m("SharedMtx");

	std::string trimString(std::string s) {
		std::string status2 = Poco::trim(s);
		if (status2.find('\0') != std::string::npos) {
			status2 = status2.substr(0, status2.find('\0'));
		}
		return status2;
	}

	int DiagnosticsStatusMemory::size() {
		return 1 + DiagnosticsStatusMemory::MaxStringLength * DiagnosticsStatusMemory::MaxCount * 2;
	}

	DiagnosticsStatusMemory::DiagnosticsStatusMemory(char * start) {
		this->start = start;
	}

	char * DiagnosticsStatusMemory::keyData(int threadNumber) {
		return (start + 1 + (threadNumber * MaxStringLength));
	}
	char * DiagnosticsStatusMemory::stringData(int threadNumber) {
		return (start + 1 + (MaxCount * MaxStringLength) + threadNumber * MaxStringLength);
	}

	int DiagnosticsStatusMemory::records() {
		return (int) (*start);
	}

	char * DiagnosticsStatusMemory::cntPointer() {
		return start;
	}

	std::string DiagnosticsStatusMemory::data(char * dataPtr) {
		std::string value(dataPtr, DiagnosticsStatusMemory::MaxStringLength);
		return trimString(value);
	}

	DiagnosticsStatusReader::DiagnosticsStatusReader(char * start) :
			mem(start) {

	}

	Diagnostics DiagnosticsStatusReader::getDiagnosticsInfo() {
		Diagnostics t;

		try {
			Poco::NamedMutex::ScopedLock l(DiagnosticsStatusMemory::m);

			int cnt = mem.records();

			for (int i = 0; i < cnt; i++) {

				DiagnosticStatus ts = std::make_pair(DiagnosticsStatusMemory::data(mem.keyData(i)), DiagnosticsStatusMemory::data(mem.stringData(i)));
				t.push_back(ts);
			}
		} catch (Poco::Exception & e) {
			LOG_STREAM_ERROR<< "Cannot read diagnostic info" << LOG_END;
			}
		return t;
	}

	DiagnosticsStatusWriter::DiagnosticsStatusWriter(char * start) :
			mem(start) {
		try {
			Poco::NamedMutex::ScopedLock l(DiagnosticsStatusMemory::m);
			rebuildIndexes();
		} catch (Poco::Exception & e) {
			LOG_STREAM_ERROR<< "Cannot create diagnostic info" << LOG_END;
			}
		}

	void DiagnosticsStatusWriter::clear() {
		try {
			Poco::NamedMutex::ScopedLock l(DiagnosticsStatusMemory::m);
			memset(mem.cntPointer(), 0, DiagnosticsStatusMemory::size());
			rebuildIndexes();
		} catch (Poco::Exception & e) {
			LOG_STREAM_ERROR<< "Cannot clear diagnostic info" << LOG_END;
			}
		}

	void DiagnosticsStatusWriter::rebuildIndexes() {
		indexes.clear();
		int cnt = mem.records();
		for (int i = 0; i < cnt; i++) {
			indexes[DiagnosticsStatusMemory::data(mem.keyData(i))] = i;
		}
	}

	void DiagnosticsStatusWriter::writeStatus(std::string key, std::string status) {
		try {
			Poco::NamedMutex::ScopedLock l(DiagnosticsStatusMemory::m);

			rebuildIndexes();

			if (indexes.find(key) == indexes.end()) {
				char * v = mem.cntPointer();
				*v = (*v) + 1;
				int size = indexes.size();
				indexes[key] = size;
			}

			if (status.length() >= DiagnosticsStatusMemory::MaxStringLength) {
				status = status.substr(0, DiagnosticsStatusMemory::MaxStringLength - 1);
			}
			if (key.length() >= DiagnosticsStatusMemory::MaxStringLength) {
				key = key.substr(0, DiagnosticsStatusMemory::MaxStringLength - 1);
			}

			//clear
			memset(mem.stringData(indexes[key]), 0, DiagnosticsStatusMemory::MaxStringLength);
			//write real data
			sprintf(mem.stringData(indexes[key]), "%s", status.c_str());

			//clear
			memset(mem.keyData(indexes[key]), 0, DiagnosticsStatusMemory::MaxStringLength);
			//write real data
			sprintf(mem.keyData(indexes[key]), "%s", key.c_str());
		} catch (Poco::Exception & e) {
			LOG_STREAM_WARNING<< "Cannot write diagnostic info (" << key << ":" << status << ")" << LOG_END;
			}
		}
	void DiagnosticsStatusWriter::writeThread(std::string status) {
		std::string tname = "<>";
		Poco::Thread * thread = Poco::Thread::current();
		if (thread) {
			tname = "<" + thread->name() + ">";
		}
		status = tname + status;

		int tid = 0;

#ifndef __WIN32__
		tid = (pid_t) syscall(SYS_gettid);
#endif
		this->writeStatus("Thread: " + Poco::NumberFormatter::format(tid), status);
	}

	Poco::Mutex DiagnosticsMemory::m;

	DiagnosticsMemory & DiagnosticsMemory::instance() {
		Poco::Mutex::ScopedLock l(m);
		static DiagnosticsMemory mem;
		return mem;
	}

	DiagnosticsMemory::DiagnosticsMemory() :
			writer_(NULL), reader_(NULL) {
		/*
		 Poco::File f("/run/user/sharedmem.dat");
		 if (!f.exists() || f.getSize() < ThreadStatusMemory::size()){
		 std::cout << "create file" << std::endl;
		 std::ofstream ofs(f.path().c_str(), std::ios::binary | std::ios::out);
		 ofs.seekp(( ThreadStatusMemory::size()));
		 ofs.write("", 1);
		 std::cout << "create file done" << std::endl;
		 }

		 std::cout << "shared mem" << std::endl;
		 */
		//mem = Poco::SharedMemory(f, (Poco::SharedMemory::AccessMode)(Poco::SharedMemory::AM_READ | Poco::SharedMemory::AM_WRITE));
		try {
			buffer = NULL;
			mem = Poco::SharedMemory("ThreadSharedMem", DiagnosticsStatusMemory::size(), (Poco::SharedMemory::AccessMode) (Poco::SharedMemory::AM_READ | Poco::SharedMemory::AM_WRITE));
			writer_ = new DiagnosticsStatusWriter(mem.begin());
			reader_ = new DiagnosticsStatusReader(mem.begin());
		} catch (Poco::Exception & e) {
			LOG_STREAM_WARNING<< "Cannot create diagnostic info, buffer workaround" << LOG_END;
				buffer = new char[DiagnosticsStatusMemory::size()];
				writer_ = new DiagnosticsStatusWriter(buffer);
				reader_ = new DiagnosticsStatusReader(buffer);
			}
		}

	DiagnosticsMemory::~DiagnosticsMemory() {
		delete writer_;
		delete reader_;
		if (buffer) {
			delete[] buffer;
		}
	}

	DiagnosticsStatusWriter & DiagnosticsMemory::writer() {
		return *this->writer_;
	}
	DiagnosticsStatusReader & DiagnosticsMemory::reader() {
		return *this->reader_;
	}

} /* namespace TBS */
