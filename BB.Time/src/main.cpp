#include <BB/Configuration.h>
#include <cstdlib>
#include <sstream>
#include "TBS/Log.h"

int main(int argc, char** argv) {

	TBS::initLogs("bb.time", 8, "/tmp/");

	int sleepTime = 10;
	int checkInterval = 60*60*3;
	int currentSleep = checkInterval - 120;

	bool finished = false;
	while (!finished){
		currentSleep += sleepTime;
		if (currentSleep > checkInterval){
			currentSleep = 0;
			std::stringstream s;
			s << "ntpdate -b -s -u pool.ntp.org";
			system(s.str().c_str());
			LDEBUG("TimeSync") << s.str() << LE;
		}
		Poco::Thread::sleep(sleepTime*1000);
	}

	return 0;
}
