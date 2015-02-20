/*
 * NodeApp.h
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#ifndef NodeApp_H_
#define NodeApp_H_

#include "Poco/Util/ServerApplication.h"
#include "Poco/Timer.h"
#include "TBS/SimpleTimer.h"
#include "BB/Node/INode.h"
#include "BB/Node/NodeManager.h"
#include <vector>

namespace BB {

	class NodeApp: public Poco::Util::ServerApplication {

		public:
			typedef Poco::SharedPtr<NodeApp> Ptr;
			NodeApp(std::string name, INodeFactory::Ptr factory);
			virtual ~NodeApp();

		protected:

			int main(const std::vector<std::string>& args);
		private:
			void onTimer(TBS::SimpleTimer::TimerArg & t);
		private:
			std::string name;

			TBS::SimpleTimer timer;

			std::map <std::string, NodeManager> nodes;

			INodeFactory::Ptr factory;
	};

} /* namespace BB */
#endif /* SensorApp_H_ */
