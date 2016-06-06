#include <BB/Camera/OpenCVGrabber.h>
#include <BB/Node/NodeTypes.h>
#include <Camera/FFMpegGrabber.h>
#include "BB/Node/Sensor/CameraNode.h"

#include <fstream>
#include <iostream>

#include <Poco/Base64Encoder.h>
#include <opencv2/opencv.hpp>
#include <Poco/Delegate.h>

#include "BB/RW.h"

namespace BB {

		static BB::Node::Settings cameraAdditional(){

			BB::Node::Settings s({
				BB::Node::Setting("frequency", "text", BB::Node::Setting::Value(3)),
				BB::Node::Setting("stream", "text"),
				BB::Node::Setting("ffmpeg", "switch", BB::Node::Setting::Value(false)),
			});

			return s;
		}

		CameraNode::CameraNode(std::string uid) :
				SwitchNode(uid, false, cameraAdditional()), t("Camera"), stopBg(false), frequency(3), cfgChanged(false) {
			t.start(*this);
			timer.Timer += Poco::delegate(this, &CameraNode::onTimer);
			this->SettingsChanged  += Poco::delegate(this, &CameraNode::onChanged);
			isffmpeg = false;
		}

		CameraNode::~CameraNode(){
			this->SettingsChanged  -= Poco::delegate(this, &CameraNode::onChanged);
			timer.Timer -= Poco::delegate(this, &CameraNode::onTimer);
			timer.stop();
			stopBg = true;
			t.join();
		}

		void CameraNode::performSwitch(bool on){
			if (on){
				int f = 10;
				{
					Poco::Mutex::ScopedLock l(mSettings);
					f = frequency;
				}

				timer.start(50, f * 1000);
			} else {
				timer.stop();
			}
		}

		void CameraNode::onChanged(SettingsValues  & s){
			updateHost();

		}

		void CameraNode::updateHost(){
			try {
				Poco::Mutex::ScopedLock l(mSettings);
				frequency = getSettings().at("frequency").convert<int>();
				streamHost = getSettings().at("stream").convert<std::string>();
				isffmpeg = getSettings().at("ffmpeg").convert<int>();
				LNOTICE("Camera") << "new cfg: " << frequency << " host: " << streamHost << LE;

				cfgChanged = true;
			} catch (...){
				LNOTICE("Camera") << "new cfg exc" << LE;
			}
		}

		void CameraNode::run(){
			Poco::Thread::sleep(2000);
			TBS::Nullable<bool> lastffmpeg;
			while (!stopBg){
				LTRACE("Camera") << "loop bg" << LE;
				bool isOk = false;
				try {
					cv::Mat mat;

					if (lastffmpeg != isffmpeg){
						LTRACE("Camera") << "reset grabber" << LE;
						lastffmpeg.set(isffmpeg);
						grabber = NULL;
					}

					if (!grabber){
						if (lastffmpeg.isEmpty() || lastffmpeg.val() == false){
							grabber = new BB::Camera::OpenCVGrabber();
						} else {
							grabber = new BB::Camera::FFMpegGrabber(this->getInfo().getUID());
						}
						Poco::Mutex::ScopedLock l(mSettings);
						LNOTICE("Camera") << "try to open: " << streamHost << LE;
						cfgChanged = false;
						grabber->open(streamHost);
					}
					LNOTICE("Camera") << "before read" << LE;
					bool res = grabber->read(mat);
					Poco::Mutex::ScopedLock l(m);
					if (res && !mat.empty()){
						LNOTICE("Camera") << "mat ok" << LE;
						this->current.set(mat);
						isOk = true;
					} else {
						LNOTICE("Camera") << "mat empty" << LE;
						this->current.reset();
						isOk = false;
					}

					Poco::Thread::sleep(100);
				} catch (Poco::Exception & e){
					LERROR("Camera") << "Grabber failed: " << e.displayText() << LE;
				} catch (std::exception & e){
					LERROR("Camera") << "Grabber failed: " << e.what() << LE;
				}
				if (!isOk){
					grabber = NULL;
					//sleep for 60s
					for (int i = 0; i < 60*5; i++){
						Poco::Thread::sleep(1000);
						if (stopBg || cfgChanged){
							break;
						}

					}
				}
			}
		}


		void CameraNode::onTimer(TBS::SimpleTimer::TimerArg & a){
			TBS::Nullable <EventLogMessage> e;
			{
				Poco::Mutex::ScopedLock l(m);
				if (current.isSet()){
					EventLogMessage msg(
							"camera snapshot",
							"image",
							RW::image2string(current.cref())
					);
					e.set(msg);
				}
			}
			if (e.isSet()){
				this->EventLogRaised.notify(this, e.ref());
			}

		}

} /* namespace BB */
