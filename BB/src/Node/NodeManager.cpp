#include <BB/Node/NodeManager.h>
#include "Poco/SharedPtr.h"
#include <vector>
#include <string>

#include "BB/DataRW.h"

#include "TBS/MQTT/Client.h"

#include "json/json.h"

#include "Poco/Path.h"
#include "Poco/File.h"

#include <fstream>

#include "TBS/Log.h"


namespace BB {

	static std::string stringFromValue(Poco::DynamicAny val){
			std::string payload;
			if (!val.isEmpty()) {
				payload = val.convert<std::string>();
			}
			return payload;
		}

	static Poco::DynamicAny valueFromString(std::string payload){
		Poco::DynamicAny val;
		if (!payload.empty()) {
			val = Poco::DynamicAny(payload);
		}
		return val;
	}


	NodeManager::NodeManager(INode::Ptr node) :
			node(node) {
		client = new TBS::MQTT::Client(node->getInfo().getUID());
		client->OnMessage += Poco::delegate(this, &NodeManager::onMessage);

		timer.Timer += Poco::delegate(this, &NodeManager::onTimer);

		const Node::Info & info = node->getInfo();

		//public node meta data
		//data channels
		for (const auto & sensor : info.getSensors()) {
			const Node::Sensor & s = sensor.second;

			std::string dataTopic = NodeChannel::dataTopic(info, s);

			std::string unit = s.unit;
			if (unit.empty()){
				unit = "null";
			}
			client->publish(dataTopic + "/meta/type", s.type);
			client->publish(dataTopic + "/meta/unit", unit);
		}

		std::cout << "before load" << std::endl;

		this->load();
		auto settingValues = this->node->getSettings();

		std::cout << "after load" << std::endl;

		node->SettingsChanged += Poco::delegate(this, &NodeManager::onChanged);
		node->DataChanged += Poco::delegate(this, &NodeManager::onData);

		//settings channels
		for (const auto & setting : info.getSettings()) {
			const Node::Setting & s = setting.second;

			std::string topic = NodeChannel::settingTopic(info, s);

			client->publish(topic + "/meta/type", s.type);

			for (const auto & meta : s.metaInfo) {
				client->publish(topic + "/meta/" + meta.first, meta.second);
			}

			std::cout << "new val" << std::endl;
			std::string val = "";
			auto found = settingValues.find(setting.first);
			if ((found != settingValues.end())) {
				val = stringFromValue(found->second);

			}
			client->publish(topic, val);
			//subscribe settings changes
			client->subscribe(topic + "/new");
		}



		timer.start(node->getCheckingPeriodInMs() / 20, node->getCheckingPeriodInMs());
	}

	NodeManager::~NodeManager() {

		node->SettingsChanged -= Poco::delegate(this, &NodeManager::onChanged);
		node->DataChanged -= Poco::delegate(this, &NodeManager::onData);

		timer.stop();
		timer.Timer -= Poco::delegate(this, &NodeManager::onTimer);

		client->OnMessage -= Poco::delegate(this, &NodeManager::onMessage);
		client = NULL;
	}


	static std::string filename(INode::Ptr n){
		Poco::File dir("settings/nodes/");
		if (!dir.exists()){
			dir.createDirectories();
		}
		return dir.path() + "/" + n->getInfo().getUID() + ".json";
	}

	void NodeManager::load() {
		Poco::File f(filename(this->node));
		bool shouldsave = !f.exists();
		if (f.exists()){
			std::ifstream str(f.path());
			Json::Value obj;
			Json::Reader rr;
			rr.parse(str, obj);

			auto settings = this->node->getSettings();

			for (auto & s : settings){
				if (obj.isMember(s.first)){
					s.second = valueFromString(obj[s.first].asString());
				} else {
					shouldsave = true;
				}
			}
			this->node->updateSetting(settings);
		}

		if (shouldsave){
			this->save();
		}
	}
	void NodeManager::save() {
		const auto & settings = this->node->getSettings();

		Json::Value obj(Json::objectValue);
		for (const auto & s : settings){
			obj[s.first] = Json::Value(stringFromValue(s.second));
		}
		Json::StyledWriter w;
		std::ofstream f(filename(this->node).c_str());
		f << w.write(obj);
	}

	void NodeManager::onData(bool & arg){
		std::cout << " data changed - read" << std::endl;
		this->read();
	}

	void NodeManager::read(){
		INode::AllData allData = node->read();
		std::cout << "reading done" << std::endl;
		const Node::Info & info = node->getInfo();
		for (auto & d : allData) {
			const Node::Sensor & sensor = info.getSensors().at(d.first);
			client->publish(NodeChannel::dataTopic(info, sensor), RW::json2OneLine(NodeDataRW::write(d.second)));
		}
	}

	void NodeManager::onChanged(INode::SettingsValues & v) {
		this->save();
	}


	void NodeManager::onTimer(TBS::SimpleTimer::TimerArg & arg) {
		std::cout << "timer" << std::endl;
		this->read();
	}
	void NodeManager::onMessage(TBS::MQTT::Message & arg) {
		std::cout << "on message: " << arg.topic.topic() << " payload: " << arg.payload << std::endl;
		const Node::Info & info = node->getInfo();
		for (const auto & setting : info.getSettings()) {

			const Node::Setting & s = setting.second;

			std::string topic = NodeChannel::settingTopic(info, s);

			if (arg.topic.topic() == topic + "/new") {
				//confirm new value
				Poco::DynamicAny val = valueFromString(arg.payload);
				INode::SettingsValues values = { { setting.first, val } };
				if (node->updateSetting(values)) {
					client->publish(topic, arg.payload);
				}
			}
		}

	}

} /* namespace TBS */
