#include <BB/Node/NodeTypes.h>
#include "BB/Node/Status/HardwareNode.h"

#include <fstream>
#include <iostream>

#include <Poco/Environment.h>

#include "sys/types.h"
#include "sys/sysinfo.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

static unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;

void initCPU() {
	FILE* file = fopen("/proc/stat", "r");
	fscanf(file, "cpu %Ld %Ld %Ld %Ld", &lastTotalUser, &lastTotalUserLow, &lastTotalSys, &lastTotalIdle);
	fclose(file);
}

double getCurrentCPU() {
	double percent;
	FILE* file;
	unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

	file = fopen("/proc/stat", "r");
	fscanf(file, "cpu %Ld %Ld %Ld %Ld", &totalUser, &totalUserLow, &totalSys, &totalIdle);
	fclose(file);

	if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow || totalSys < lastTotalSys || totalIdle < lastTotalIdle) {
		//Overflow detection. Just skip this value.
		percent = -1.0;
	} else {
		total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) + (totalSys - lastTotalSys);
		percent = total;
		total += (totalIdle - lastTotalIdle);
		percent /= total;
		percent *= 100;
	}

	lastTotalUser = totalUser;
	lastTotalUserLow = totalUserLow;
	lastTotalSys = totalSys;
	lastTotalIdle = totalIdle;

	return percent;
}

namespace BB {

	static Node::Info hwInfo(std::string uid) {

		initCPU();

		BB::Node::Sensors sensors( {
			BB::Node::Sensor("CPU", BB::Node::Info::Status, "%"),
			BB::Node::Sensor("Memory",
			BB::Node::Info::Status, "%")
		});

		BB::Node::Settings settings/*({
		 BB::Node::Setting("name"),
		 BB::Node::Setting("armed", "switch", BB::Node::Setting::Value(0)),
		 BB::Node::Setting("heating", "switch", BB::Node::Setting::Value(1)),
		 })*/;

		Node::Info info(uid, Node::Info::Status, sensors, settings);
		return info;
	}

	HardwareNode::HardwareNode(int period) :
			BasicNode(hwInfo(Poco::Environment::nodeName()), period) {
	}

	static double hround(double val){
		return floor(val * 10 + 0.5)/10;
	}

	HardwareNode::AllData HardwareNode::read() {
		std::cout << "hw read" << std::endl;

		std::cout << "OS Name: " << Poco::Environment::osName() << std::endl << "OS Version: " << Poco::Environment::osVersion() << std::endl << "OS Arch: "
				<< Poco::Environment::osArchitecture() << std::endl << "Node Name: " << Poco::Environment::nodeName() << std::endl << "Node ID: "
				<< Poco::Environment::nodeId() << std::endl;

		HardwareNode::AllData data;

		struct sysinfo memInfo;

		sysinfo(&memInfo);

		long long totalPhysMem = memInfo.totalram;
		//Multiply in next statement to avoid int overflow on right hand side...
		totalPhysMem *= memInfo.mem_unit;

		long long physMemUsed = memInfo.totalram - memInfo.freeram;
		//Multiply in next statement to avoid int overflow on right hand side...
		physMemUsed *= memInfo.mem_unit;


		data.insert(std::make_pair("Memory", Node::Data(hround(physMemUsed * 100.0 / totalPhysMem), Node::localNow())));

		data.insert(std::make_pair("CPU", Node::Data(hround(getCurrentCPU()), Node::localNow())));

		return data;
	}

} /* namespace BB */
