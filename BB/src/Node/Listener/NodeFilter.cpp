/*
 * NodeFilter.cpp
 *
 *  Created on: 2.4.2014
 *      Author: JV
 */

#include "BB/Node/Listener/NodeFilter.h"
#include "BB/Node/NodeHelpers.h"

namespace BB {

	bool sourcePass(Node::Info i, Source nodeSource){
		if (nodeSource.name.isSet()){
			if (i.getUID() != nodeSource.name.val()) return false;
		}
		if (nodeSource.type.isSet()){
			if (i.getType() != nodeSource.type.val()) return false;
		}
		return true;
	}

	bool sourcePass(Node::Sensor i, Source nodeSource){
		if (nodeSource.name.isSet()){
			if (i.name != nodeSource.name.val()) return false;
		}
		if (nodeSource.type.isSet()){
			if (i.type != nodeSource.type.val()) return false;
		}
		return true;
	}

	SingleTimespanNodeFilter::SingleTimespanNodeFilter(Source sensorKey, Poco::Timespan timespan, Collecting::Type colllecting) :
			sensorKey(sensorKey), timespan(timespan), colllectingType(colllecting) {
		//nextAcceptable = nextAcceptable + timespan;
	}

	static Node::Data resolveValue(std::vector<Node::Data> & values, Collecting::Type t) {
		Node::Data candidate = values.back();

		LDEBUG("Filter") << "resolving: " << values.size() << " type: " << t << LE;
		for (auto d : values) {
			LDEBUG("Filter") << "resolving: at# " << d << LE;
		}

		if (t == Collecting::ReturnSum) {
			double sum = 0;
			for (auto d : values) {
				sum += d.getValue();
			}
			candidate.setValue(sum);
		}

		if (t == Collecting::ReturnMin) {
			for (auto d : values) {
				if (d.getValue() < candidate.getValue()) {
					candidate.setValue(d.getValue());
					candidate.tags() = d.tags();
				}
			}
		}

		if (t == Collecting::ReturnMax) {
			for (auto d : values) {
				if (d.getValue() > candidate.getValue()) {
					candidate.setValue(d.getValue());
					candidate.tags() = d.tags();
				}
			}
		}

		if (t == Collecting::ReturnAvg) {
			double sum = 0;
			for (auto d : values) {
				sum += d.getValue();
			}
			candidate.setValue(sum / values.size());
		}

		LNOTICE("Filter") << "resolving: final: " << candidate << LE;

		return candidate;
	}

	TBS::Nullable<Node::Data> SingleTimespanNodeFilter::filter(const Node::Info & nodeInfo, const Node::Sensor & sensor, const Node::Data & data) {
		TBS::Nullable<Node::Data> filtered;
		if (!sourcePass(sensor, sensorKey)) {
			return filtered;
		}

		collected.push_back(data);

		if (data.getDate() < this->nextAcceptable) {
			return filtered;
		}

		filtered.set(resolveValue(collected, colllectingType));
		collected.clear();

		do {
			this->nextAcceptable = this->nextAcceptable + this->timespan;
		} while (data.getDate() >= this->nextAcceptable);

		return filtered;
	}

	TimespanNodeFilter::TimespanNodeFilter(NodeSource sensorType, Poco::Timespan timespan, Collecting::Type colllecting) :
		sensorSource(sensorType), timespan(timespan), colllecting(colllecting) {

	}

	TBS::Nullable<Node::Data> TimespanNodeFilter::filter(const Node::Info & nodeInfo, const Node::Sensor & sensor, const Node::Data & data) {

		TBS::Nullable<Node::Data> filtered;

		if (!sourcePass(nodeInfo, sensorSource.node)) {
			return filtered;
		}

		if (!sourcePass(sensor, sensorSource.sensor)) {
			return filtered;
		}

		std::string key = Node::nodeId(nodeInfo, sensor);
		if (filters.find(key) == filters.end()) {
			Source s;
			s.type.set(sensor.type);
			s.name.set(sensor.name);
			filters.insert(std::make_pair(key, SingleTimespanNodeFilter(s, timespan, colllecting)));
		}

		return filters.at(key).filter(nodeInfo, sensor, data);
	}

	EmptyNodeFilter::EmptyNodeFilter(NodeSource sensorSource) :
		sensorSource(sensorSource) {

	}

	TBS::Nullable<Node::Data> EmptyNodeFilter::filter(const Node::Info & nodeInfo, const Node::Sensor & sensor, const Node::Data & data) {
		TBS::Nullable<Node::Data> newdata;
		if (!sourcePass(nodeInfo, sensorSource.node)) {
			return newdata;
		}
		if (!sourcePass(sensor, sensorSource.sensor)) {
			return newdata;
		}
		newdata.set(data);
		return newdata;
	}

} /* namespace BB */
