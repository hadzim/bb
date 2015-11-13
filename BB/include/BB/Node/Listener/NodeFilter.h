/*
 * Filter.h
 *
 *  Created on: 2.4.2014
 *      Author: JV
 */

#ifndef NODEFILTER_H_
#define NODEFILTER_H_
#include <Poco/SharedPtr.h>
#include <Poco/DateTime.h>
#include <Poco/Timespan.h>
#include "TBS/Nullable.h"
#include "BB/Node/INode.h"
#include "BB/Node/NodeHelpers.h"
#include <vector>

namespace BB {

	class INodeFilter {
		public:
			typedef Poco::SharedPtr<INodeFilter> Ptr;
			typedef std::vector <Ptr> PtrList;
			virtual ~INodeFilter() {
			}
			virtual TBS::Nullable<Node::Data> filter(const Node::Info & nodeInfo, const Node::Sensor & sensor, const Node::Data & data) = 0;
	};

	struct Collecting {
			enum Type {
				ReturnLast,
				ReturnAvg,
				ReturnSum,
				ReturnMax,
				ReturnMin,
			};

	};

	struct Source {
			TBS::Nullable <std::string> name;
			TBS::Nullable <std::string> type;
	};

	struct NodeSource {
			Source node;
			Source sensor;
	};

	bool sourcePass(Node::Info i, Source nodeSource);
	bool sourcePass(Node::Sensor i, Source nodeSource);

	class SingleTimespanNodeFilter : public INodeFilter {
		public:
			SingleTimespanNodeFilter(Source sensorKey, Poco::Timespan timespan, Collecting::Type colllecting = Collecting::ReturnLast);

			TBS::Nullable<Node::Data> filter(const Node::Info & nodeInfo, const Node::Sensor & sensor, const Node::Data & data);
		private:
			Source sensorKey;
			Poco::Timespan timespan;
			Poco::DateTime nextAcceptable;
			std::vector <Node::Data> collected;
			Collecting::Type colllectingType;
	};

	class TimespanNodeFilter : public INodeFilter {
		public:
			TimespanNodeFilter(NodeSource sensor, Poco::Timespan timespan, Collecting::Type colllecting = Collecting::ReturnLast);

			TBS::Nullable<Node::Data> filter(const Node::Info & nodeInfo, const Node::Sensor & sensor, const Node::Data & data);
		private:
			NodeSource sensorSource;
			Poco::Timespan timespan;
			Collecting::Type colllecting;
			std::map <std::string, SingleTimespanNodeFilter> filters;

	};

	class EmptyNodeFilter : public INodeFilter {
		public:
			EmptyNodeFilter(NodeSource sensor);

			TBS::Nullable<Node::Data> filter(const Node::Info & nodeInfo, const Node::Sensor & sensor, const Node::Data & data);
		private:
			NodeSource sensorSource;
	};




} /* namespace BB */
#endif /* FILTER_H_ */
