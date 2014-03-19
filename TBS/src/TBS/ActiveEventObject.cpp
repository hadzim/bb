#include "TBS/ActiveEventObject.h"
#include "TBS/Log.h"
namespace TBS {

	ActiveEventObject::ActiveEventObject() :
			active(false) {
	}
	ActiveEventObject::~ActiveEventObject() {
		Poco::Mutex::ScopedLock(this->mutex);
		poco_assert_dbg(this->active == false);
	}

	bool ActiveEventObject::isActive() {
		//Poco::Mutex::ScopedLock(this->mutex);
		return active;
	}

	void ActiveEventObject::activate() {
		//LOG_STREAM_DEBUG << "active lock" << LOG_STREAM_END
		{

			Activation act = Activating;

			Poco::Mutex::ScopedLock(this->mutex);
			if (!this->active) {
					LOG_STREAM_DEBUG<< "before active event " << (this->BeforeActivate.empty() ? "none" : "some") << LOG_STREAM_END
					this->OuterActivation(this, act);
					this->BeforeActivate(this, this->active);

					this->active = true;
					LOG_STREAM_DEBUG << "after active event " << (this->AfterActivate.empty() ? "none" : "some") << LOG_STREAM_END
					this->AfterActivate(this, this->active);
					this->InnerActivation(this, act);
				}
			}
			//LOG_STREAM_DEBUG << "after active lock" << LOG_STREAM_END
		}
	void ActiveEventObject::passivate() {
		Activation act = Passivating;
		Poco::Mutex::ScopedLock(this->mutex);
		if (this->active) {
			this->InnerActivation(this, act);
			this->BeforePassivate(this, this->active);
			this->active = false;
			this->AfterPassivate(this, this->active);
			this->OuterActivation(this, act);
		}
	}

}

