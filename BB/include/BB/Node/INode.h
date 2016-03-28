/*
 * ISensor.h
 *
 *  Created on: Feb 1, 2013
 *      Author: root
 */

#ifndef INODE_H_
#define INODE_H_
#include <BB/Node/NodeTypes.h>
#include <Poco/BasicEvent.h>
#include "Poco/SharedPtr.h"
#include <vector>
#include <string>
#include <sstream>
#include <Poco/Exception.h>

#include "Poco/Message.h"

namespace BB {

	class Project {
		public:
			static std::string projectID(){
				return "BB@Default";
			}
	};


	class NodeChannel {
		public:
			static std::string projectTopic(){
				std::stringstream s;
				s << "devices/" << Project::projectID();
				return s.str();
			}

			static std::string nodeTopic(const Node::Info & info){
				std::stringstream s;
				s << projectTopic() << "/nodes/" << info.getUID() << "";
				return s.str();
			}

			static std::string dataTopic(const Node::Info & info, const Node::Sensor & sensor){
				std::stringstream s;
				s << nodeTopic(info) << "/data/" << sensor.name << "";
				return s.str();
			}

			static std::string settingTopic(const Node::Info & info, const Node::Setting & s){
				std::stringstream str;
				str << nodeTopic(info) << "/settings/" << s.name << "";
				return str.str();
			}

			static std::string eventLogTopic(){
				std::stringstream str;
				str << projectTopic() << "/eventlog";
				return str.str();
			}
	};

	class INode {
		public:
			typedef Poco::SharedPtr <INode> Ptr;
			typedef std::vector<Ptr> PtrList;

			virtual ~INode(){}

			virtual Node::Info getInfo() = 0;
			virtual int getCheckingPeriodInMs() = 0;

			typedef std::map <std::string, Node::Data> AllData;
			virtual AllData read() = 0;

			typedef std::map <std::string, Node::Setting::Value> SettingsValues;

			virtual SettingsValues getSettings() = 0;
			virtual bool updateSetting(SettingsValues newValues) = 0;
			Poco::BasicEvent <SettingsValues> SettingsChanged;
			//node can notify, when it wants to be read
			Poco::BasicEvent <bool> DataChanged;


			struct EventLogMessage {
					Poco::DateTime time;
					std::string message;
					std::string dataType;
					std::string data;
					int level;

					EventLogMessage(std::string message, std::string dataType = "", std::string data = "", int level = Poco::Message::PRIO_INFORMATION)
						: message(message), dataType(dataType), data(data), level(level) {

					}
			};

			Poco::BasicEvent <EventLogMessage> EventLogRaised;

			struct AdditionalInfo {
					std::string topic;
					std::string payload;
			};

			Poco::BasicEvent <AdditionalInfo> AdditionalInfoRaised;

	};


	#define API

	POCO_DECLARE_EXCEPTION(API, NoDataException, Poco::Exception)

} /* namespace TBS */

std::ostream & operator<<(std::ostream & o, const BB::INode::EventLogMessage & message);

#endif /* ISENSOR_H_ */
