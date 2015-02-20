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
		//LOG_STREAM_DEBUG << "active lock" << LE
		{

			Activation act = Activating;

			Poco::Mutex::ScopedLock(this->mutex);
			if (!this->active) {
					this->OuterActivation(this, act);
					this->BeforeActivate(this, this->active);

					this->active = true;
					this->AfterActivate(this, this->active);
					this->InnerActivation(this, act);
				}
			}
			//LOG_STREAM_DEBUG << "after active lock" << LE
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

