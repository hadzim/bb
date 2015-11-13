#include <BB/Node/INode.h>

namespace BB {

	namespace Node {
		std::string nodeId(const Info & info, const Sensor & sensor){
			return "Node::" + info.getUID() + "::Sensor::" + sensor.name;
		}
	}

}  // namespace BB
