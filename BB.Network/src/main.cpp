#include <BB/Configuration.h>
#include <cstdlib>
#include <sstream>

int main(int argc, char** argv) {
	std::string type = BB::Configuration::initCfg("Network", "Type", "Static");
	std::string ip = BB::Configuration::initCfg("Network", "IP", "192.168.0.99");
	std::string broadcast = BB::Configuration::initCfg("Network", "Broadcast", "192.168.0.255");
	std::string gw = BB::Configuration::initCfg("Network", "Gateway", "192.168.0.1");


	{
		std::stringstream s;
		s << "ip link set dev eth0 up";
		system(s.str().c_str());
	}

	if (type == "Static"){
		{
			std::stringstream s;
			s << "ip addr add " << ip << "/24 broadcast " << broadcast << " dev eth0";
			system(s.str().c_str());
		}
		{
			std::stringstream s;
			s << "ip route add default via " << gw;
			system(s.str().c_str());
		}


	}




	return 0;
}
