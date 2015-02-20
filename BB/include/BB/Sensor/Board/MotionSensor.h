/*
 * MotionSensor.h
 *
 *  Created on: Feb 3, 2013
 *      Author: root
 */

#ifndef MotionSensor_H_
#define MotionSensor_H_
#include <exploreBB/gpio/GPIO.h>
#include "BB/Sensor/ISensor.h"

#include "TBS/Nullable.h"

#include "TBS/SimpleTimer.h"

namespace BB {

	class MotionSensorPins {
			public:
				MotionSensorPins(int slot);
			public:

				int out1;
				int out2;
				int out3;

				int in1;
				int in2;
				int in3;
		};

	class MotionSensor : public ISensor {
		public:
			MotionSensor(int slot);
			virtual ~MotionSensor();

			virtual Requests getRequests();
			virtual int getPeriodInMs();
			virtual std::string getName();
		private:
			void onMotionTimer(TBS::SimpleTimer::TimerArg &);
			void onConnectionTimer(TBS::SimpleTimer::TimerArg &);
			bool isConnected();
			void setConnected(bool);
		private:
			int slot;
			bool isConnected_;

			MotionSensorPins pins;
			Poco::Mutex m;
			TBS::SimpleTimer motionTimer;
			TBS::SimpleTimer connectionTimer;
			typedef std::pair <int, int> ActiveAndTotal;
			TBS::Nullable <ActiveAndTotal> ticks;

	};


	class MotionSensorBBB : public ISensor {
		public:
			MotionSensorBBB(int pin);
			virtual ~MotionSensorBBB();

			virtual Requests getRequests();
			virtual int getPeriodInMs();
			virtual std::string getName();
		private:
			void onMotionTimer(TBS::SimpleTimer::TimerArg &);
			void onConnectionTimer(TBS::SimpleTimer::TimerArg &);
			bool isConnected();
			void setConnected(bool);
		private:
			int pin;
			bool isConnected_;
			exploringBB::GPIO gpio;
			Poco::Mutex m;
			TBS::SimpleTimer motionTimer;
			typedef std::pair <int, int> ActiveAndTotal;
			TBS::Nullable <ActiveAndTotal> ticks;

	};


} /* namespace BB */
#endif /* MotionSensor_H_ */
