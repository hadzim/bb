/*
 * RemoteClientForwarder.h
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#ifndef RemoteClientForwarder_H_
#define RemoteClientForwarder_H_
#include "Poco/SharedPtr.h"
#include "../Cache.h"
#include "../IForwarder.h"

#include "TBS/Log.h"

#include "Poco/Thread.h"

#include "Poco/Runnable.h"

#include "BB/Forwarder/RemoteClient/RemoteClient.h"

namespace BB {


	class RemoteClientForwarder: public IForwarder, public Poco::Runnable {
		public:
			typedef Poco::SharedPtr<RemoteClientForwarder> Ptr;
			RemoteClientForwarder(const RemoteClientSettings & settings);
			virtual ~RemoteClientForwarder();

			void forward(const SensorData & data);
			void forward(const RuntimeStatus & data);
			void forward(const Task & data);
			void forward(const Notification & data);
		private:
			void run();
		private:
			RemoteClient client;

			bool stopped;
			Poco::Thread t;

	};

} /* namespace BB */
#endif /* RemoteClientForwarder_H_ */
