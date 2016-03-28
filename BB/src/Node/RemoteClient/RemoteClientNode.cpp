/*
 * RemoteClientNode.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: dron
 */
#include <BB/Node/Listener/BgNodeForwarder.h>
#include <BB/Node/Listener/NodeListener.h>
#include <BB/Node/RemoteClient/RemoteClientNode.h>
#include <BB/Node/RemoteClient/RemoteClientNodeForwarder.h>


namespace BB {

	static Node::Info rcInfo(std::string uid) {

		BB::Node::Sensors sensors({
			//BB::Node::Sensor("Online", BB::Node::Info::Status, ""),
			BB::Node::Sensor("Pending", BB::Node::Info::Status, "")
		});

		BB::Node::Settings settings({
		 BB::Node::Setting("name"),
		 BB::Node::Setting("url", "text", BB::Node::Setting::Value("localhost")),
		 BB::Node::Setting("port", "text", BB::Node::Setting::Value(81)),
		 BB::Node::Setting("query", "text", BB::Node::Setting::Value("/mereni/www/")),
		 BB::Node::Setting("projectID", "text", BB::Node::Setting::Value("1")),
	    });

		Node::Info info(uid, Node::Info::Status, sensors, settings);
		return info;
	}


	RemoteClientNode::RemoteClientNode(std::string uid) : BasicNode(rcInfo(uid), 30 * 1000), currentConnectionStatus(false), pendingQueue(0) {


		//LDEBUG("RemoteClient") << "URL:" << s.url << ":" << s.port << s.query << " project: " << s.projectID << LE;

		this->rc = new RemoteClientNodeForwarder(retrieveSettings());
		this->rc->remoteClient().OnlineChanged += Poco::delegate(this, &RemoteClientNode::onConnectionStatus);
		this->rc->remoteClient().CacheChanged += Poco::delegate(this, &RemoteClientNode::onPendingQueue);

		this->SettingsChanged += Poco::delegate(this, &RemoteClientNode::onChanged);

		auto bg = new BgNodeForwarder(uid+"-bg", this->rc);
		//filter only temperature each x minutes - take care of timespan format !!!
		this->filter = new FilteringNodeForwarder(bg);
		{
			NodeSource s;
			s.sensor.type.set(Node::Info::Temperature);
			this->filter->addFilter(
				new TimespanNodeFilter(s, Poco::Timespan(0, 0, 5, 0, 0), Collecting::ReturnAvg)
			);
		}
		{
			NodeSource s;
			s.sensor.type.set(Node::Info::Motion);
			this->filter->addFilter(
				new TimespanNodeFilter(s, Poco::Timespan(0, 0, 5, 0, 0), Collecting::ReturnMax)
			);
		}
		{
			NodeSource s;
			s.sensor.type.set(Node::Info::Contact);
			this->filter->addFilter(
				new TimespanNodeFilter(s, Poco::Timespan(0, 0, 5, 0, 0), Collecting::ReturnMin)
			);
		}
		{
			NodeSource s;
			s.sensor.type.set(Node::Info::Camera);
			this->filter->addFilter(
				new EmptyNodeFilter(s)
			);
		}
		{
			NodeSource s;
			s.sensor.name.set("Alarm");
			this->filter->addFilter(
				new TimespanNodeFilter(s, Poco::Timespan(0, 0, 5, 0, 0), Collecting::ReturnMax)
			);
		}



		listener = new NodeListener(this->filter, uid);

		LDEBUG("RemoteClient") << "construct finished" << LE;
	}



	RemoteClientSettings RemoteClientNode::retrieveSettings(){

		RemoteClientSettings rs(
						this->getSettings().at("url").convert<std::string>(),
						this->getSettings().at("port").convert<int>(),
						this->getSettings().at("query").convert<std::string>(),
						this->getSettings().at("projectID").convert<std::string>()
				);

		LNOTICE("RemoteClient") << "new endpoint: " << rs.url << ":" << rs.port << rs.query << " project: " << rs.projectID << LE;
		return rs;
	}


	RemoteClientNode::~RemoteClientNode() {
		LDEBUG("RemoteClient") << "destruct finished" << LE;
		this->rc->remoteClient().OnlineChanged -= Poco::delegate(this, &RemoteClientNode::onConnectionStatus);
		this->rc->remoteClient().CacheChanged -= Poco::delegate(this, &RemoteClientNode::onPendingQueue);
		this->SettingsChanged -= Poco::delegate(this, &RemoteClientNode::onChanged);
	}

	void RemoteClientNode::onChanged(SettingsValues  & s){

		this->rc->remoteClient().setEndpoint(retrieveSettings());
		bool reread = true;
		this->DataChanged.notify(this, reread);
	}

	void RemoteClientNode::onConnectionStatus(bool & isOnline){
		LNOTICE("RemoteClient") << "new online status: " << isOnline << LE;
		currentConnectionStatus = isOnline;
	}

	void RemoteClientNode::onPendingQueue(int & pq){
		LNOTICE("RemoteClient") << "new pending queue count: " << pq << LE;
		pendingQueue = pq;
	}


	RemoteClientNode::AllData RemoteClientNode::read() {
		LDEBUG("RemoteClient") << "remote client read" << LE;

		AllData data;
		auto now = Node::localNow();

		std::string tag = currentConnectionStatus ? "online" : "offline";

		Node::Data pendingData(pendingQueue, now);
		pendingData.tags().insert(tag);

		data.insert(std::make_pair("Pending", pendingData));

		return data;
	}



} /* namespace BB */
