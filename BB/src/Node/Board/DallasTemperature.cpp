#include "BB/Node/Board/DallasTemperature.h"

#include <fstream>
#include <iostream>
#include <Poco/Glob.h>
#include <Poco/Path.h>

namespace BB {
	namespace DallasTemperature {

		std::string Helpers::dir() {
			return "/sys/bus/w1/devices/";
		}
		std::string Helpers::file(std::string uid) {
			return dir() + uid + "/w1_slave";
		}

		Node::Node(BB::Node::Info info, int period) :
				SensorNode(info, period) {

		}

		BB::Node::Data Node::readOne(const BB::Node::Info & info, const BB::Node::Sensor & sensor) {

			std::ifstream t(Helpers::file(info.getUID()));

			std::string last;
			while (t.good()) {
				t >> last;
			}

			//std::cout << "sensor: " << sensorID  << " " << last << std::endl;
			int temp = 0;
			if (sscanf(last.c_str(), "t=%d", &temp) != 1) {
				throw Poco::Exception("Cannot parse temperature");
			}
			double val = temp * 1.0 / 1000.0;

			//Sensor::Info info(sensorID, Sensor::Info::Temperature, Sensor::Info::UnitTemperature);
			BB::Node::Data data(val, BB::Node::localNow());
			return data;
		}

		int Factory::getCheckingPeriodInMs() {
			return 90 * 1000;
		}
		INode::PtrList Factory::getNodes() {
			INode::PtrList nodes;

			std::set<std::string> files;
			Poco::Glob::glob(Helpers::dir() + "28*", files);

			BB::Node::Sensors sensors({ BB::Node::Sensor("Temperature", BB::Node::Info::Temperature, "C") });

			BB::Node::Settings settings({ BB::Node::Setting("name"), BB::Node::Setting("place") });

			for (std::set<std::string>::iterator it = files.begin(); it != files.end(); ++it) {
				Poco::Path path(*it);

				std::string nodeUID = path[path.depth() - 1];

				SensorNode::Ptr sensorNode = new Node(BB::Node::Info(nodeUID, BB::Node::Info::Temperature, sensors, settings), 60);
				nodes.push_back(sensorNode);
			}
			return nodes;
		}

	}

} /* namespace BB */
