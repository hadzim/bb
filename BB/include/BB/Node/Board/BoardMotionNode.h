/*
 * BoardMotionNode.h
 *
 *  Created on: Dec 10, 2015
 *      Author: dron
 */

#ifndef BOARDMOTIONNODE_H_
#define BOARDMOTIONNODE_H_
#include <exploreBB/gpio/GPIO.h>
#include "BB/Node/Sensor/MotionNode.h"

#include "TBS/SimpleTimer.h"

#include "TBS/Nullable.h"
#include <utility>

namespace BB {

	class BoardMotionNode : public MotionNode {
		public:
			BoardMotionNode(std::string uid);
			virtual ~BoardMotionNode();

			void onMotionTimer(TBS::SimpleTimer::TimerArg &);
		protected:
			void afterRead(Node::Data & data, const Node::Info & info, const Node::Sensor & sensor);
			BB::Node::Data readOne(const BB::Node::Info & info, const BB::Node::Sensor & sensor);
		private:
			TBS::Nullable<int> pin;
			//exploringBB::GPIO gpio;
			Poco::Mutex m;
			TBS::SimpleTimer motionTimer;
			typedef std::pair <int, int> ActiveAndTotal;
			TBS::Nullable <ActiveAndTotal> ticks;
	};

} /* namespace BB */

#endif /* BOARDMOTIONNODE_H_ */
