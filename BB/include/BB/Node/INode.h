/*
 * ISensor.h
 *
 *  Created on: Feb 1, 2013
 *      Author: root
 */

#ifndef INODE_H_
#define INODE_H_
#include <BB/Node/NodeTypes.h>
#include "Poco/SharedPtr.h"
#include <vector>
#include <string>

namespace BB {

	class Project {
		public:
			static std::string projectID(){
				return "BB@Default";
			}
	};


	class NodeChannel {
		public:
			static std::string nodeTopic(const Node::Info & info){
				std::stringstream s;
				s << "devices/" << Project::projectID() << "/nodes/" << info.getUID() << "/";
				return s.str();
			}

			static std::string dataTopic(const Node::Info & info, const Node::DataStream & dataStream){
				std::stringstream s;
				s << nodeTopic(info) << "data/" << dataStream.name << "/";
				return s.str();
			}

			static std::string settingTopic(const Node::Info & info, const Node::Setting & s){
				std::stringstream str;
				str << nodeTopic(info) << "setting/" << s.name << "/";
				return str.str();
			}
	};

	class INode {
		public:
			typedef Poco::SharedPtr <INode> Ptr;
			typedef std::vector<Ptr> PtrList;

			virtual ~INode();

			virtual Node::Info getInfo() = 0;
			virtual int getCheckingPeriodInMs() = 0;

			typedef std::map <std::string, Node::Data> AllData;
			virtual AllData read() = 0;
	};

	class INodeFactory {
		public:

			typedef Poco::SharedPtr <INodeFactory> Ptr;

			virtual ~INodeFactory(){}

			virtual int getCheckingPeriodInMs() = 0;
			virtual INode::PtrList getNodes() = 0;
	};

} /* namespace TBS */
#endif /* ISENSOR_H_ */
