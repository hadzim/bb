#include <BB/Configuration.h>
#include <cstdlib>
#include <sstream>
#include "BB/Forwarder/IForwarder.h"
#include "BB/ForwardApp.h"
#include "BB/App.h"
#include "BB/Service/DataSender.h"
#include "BB/Task/CaptureCameraTask.h"

namespace BB {
	class ActionsForwarder: public IForwarder {
		public:
			typedef Poco::SharedPtr<ActionsForwarder> Ptr;

			void forward(const SensorData & data) {
				if (data.getType() == SensorData::Motion) {
					this->currentMotion = data.getValue();
					this->detect();
				}

			}
			void forward(const RuntimeStatus & data) {
				this->currentStatus.update(data);
				this->detect();
			}

			virtual void forward(const Task & t) {
			}
			virtual void forward(const Notification & t) {
			}

		private:
			void detect() {
				long minspan = 1000l * 1000l * 30l;

				if (lastAction.elapsed() < minspan) {
					return;
				}

				if (currentStatus.getStatus(RuntimeStatus::IndexAlarm) != RuntimeStatus::AlarmArmed) {
					return;
				}

				if (currentMotion < 5) {
					return;
				}


				sender.send(CaptureCameraTask::create(5, 3));

				Notification notification(Notification::Alert, "MotionDetector", "Motion when armed");
				sender.send(notification);
			}
		private:
			Poco::Timestamp lastAction;
			RuntimeStatus currentStatus;
			int currentMotion;

			DataSender sender;
	};

	class ActionFactory: public IForwarderFactory {
			virtual IForwarder::Ptr createForwarder() {
				return new ActionsForwarder();
			}
	};
}

FWD_BB_MAIN("Actions", BB::ActionFactory)
