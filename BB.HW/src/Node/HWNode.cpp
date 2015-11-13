#include <BB/Node/NodeTypes.h>
#include "HWNode.h"
#include <Poco/Delegate.h>
#include <fstream>
#include <iostream>

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


		static Node::Info hwInfo(UI::Ptr ui, std::string uid){


			BB::Node::Settings::List l = {
					BB::Node::Setting("backlight", "range", BB::Node::Setting::Value(50)),
			};

			for (auto n : ui->getNames()){
				l.push_back(BB::Node::Setting("screen-" + n, "switch", BB::Node::Setting::Value(1)));
			}

			initCPU();

			BB::Node::Sensors sensors( { BB::Node::Sensor("CPU", BB::Node::Info::Status, "%"), BB::Node::Sensor("Memory", BB::Node::Info::Status, "%") });
			BB::Node::Settings settings(l);

			Node::Info info(uid, Node::Info::Status, sensors, settings);
			return info;
		}
/*
		void ScreenNode::afterRead(Node::Data & data, const Node::Info & info, const Node::Sensor & sensor){
			auto settings = getSettings();
			std::string tag = "calm";

			double thresh = 0.0;
			if (Node::isFilled(settings.at("thresh"), thresh) && data.getValue() >= thresh){
				tag = "motion";
			}
			data.set("tag", tag);
		}
*/
		HWNode::HWNode(UI::Ptr ui) :
				BasicNode(hwInfo(ui, "screen"), 20*1000), ui(ui) {
			this->SettingsChanged += Poco::delegate(this, &HWNode::onChanged);
		}

		HWNode::~HWNode(){
			this->SettingsChanged -= Poco::delegate(this, &HWNode::onChanged);
		}

		static double hround(double val){
			return floor(val * 10 + 0.5)/10;
		}

		HWNode::AllData HWNode::read(){

			std::cout << "hw read" << std::endl;

			std::cout << "OS Name: " << Poco::Environment::osName() << std::endl << "OS Version: " << Poco::Environment::osVersion() << std::endl << "OS Arch: "
					<< Poco::Environment::osArchitecture() << std::endl << "Node Name: " << Poco::Environment::nodeName() << std::endl << "Node ID: "
					<< Poco::Environment::nodeId() << std::endl;


			HWNode::AllData data;

			struct sysinfo memInfo;

			sysinfo(&memInfo);

			long long totalPhysMem = memInfo.totalram;
			totalPhysMem *= memInfo.mem_unit;

			long long physMemUsed = memInfo.totalram - memInfo.freeram;
			physMemUsed *= memInfo.mem_unit;

			data.insert(std::make_pair("Memory", Node::Data(hround(physMemUsed * 100.0 / totalPhysMem), Node::localNow())));

			data.insert(std::make_pair("CPU", Node::Data(hround(getCurrentCPU()), Node::localNow())));


			auto settings = this->getSettings();

			for (auto n : ui->getNames()){
				int val = 0.0;
				if (Node::isFilled(settings.at("screen-" + n), val)){
					if (val != 0){
						ui->unignore(n);
					} else {
						ui->ignore(n);
					}
				}
			}

			int intensity = 0;
			if (Node::isFilled(settings.at("backlight"), intensity)){
				ui->setIntesity(intensity);
			}

			return data;
		}

		void HWNode::onChanged(SettingsValues  & s){
			std::cout << "Screen data changed" << std::endl;
			bool reread = true;
			this->DataChanged.notify(this, reread);
		}


} /* namespace BB */
