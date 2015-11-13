#include <BB/Node/NodeTypes.h>
#include "BB/Node/Sensor/CameraNode.h"

#include <fstream>
#include <iostream>

#include <Poco/Base64Encoder.h>
#include <opencv2/opencv.hpp>

namespace BB {

		static Node::Info cameraInfo(std::string uid){

			BB::Node::Sensors sensors(
					{
						BB::Node::Sensor("LiveStream", BB::Node::Info::Camera, ""),
						BB::Node::Sensor("AlarmStream", BB::Node::Info::Camera, "")
					}
			);

			BB::Node::Settings settings({
				BB::Node::Setting("name"),
				BB::Node::Setting("place"),
				BB::Node::Setting("security", "switch", BB::Node::Setting::Value(false)),
			});

			Node::Info info(uid, Node::Info::Camera, sensors, settings);
			return info;

		}

		CameraNode::CameraNode(Camera::IGrabber::Ptr grabber, std::string uid, int period) :
				BasicNode(cameraInfo(uid), period), grabber(grabber), t("Camera"), stopBg(false) {
			t.start(*this);
		}

		CameraNode::~CameraNode(){
			stopBg = true;
			t.join();
		}

		void CameraNode::run(){
			while (!stopBg){
				cv::Mat mat;
				bool res = grabber->read(mat);
				Poco::Mutex::ScopedLock l(m);
				if (res){
					this->current.set(mat);
				} else {
					this->current.reset();
				}
				Poco::Thread::sleep(10);
			}
		}

		static std::vector<unsigned char> mat2Bytes(cv::Mat & src){
		    std::vector<unsigned char> outVet;
		    imencode(".png", src, outVet);
		    return outVet;
		}

		static std::string streamcv(cv::Mat & m){

			std::stringstream s;
			s << "data:image/png;base64,";
			Poco::Base64Encoder encoder(s);
			encoder.rdbuf()->setLineLength(0);

			for (auto b : mat2Bytes(m)){
				encoder << b;
			}

			return s.str();
		}

		CameraNode::AllData CameraNode::read(){
			//if is alarm - grab also alarm stream
			//else only live stream

			CameraNode::AllData data;

			Poco::DateTime dt = Node::localNow();

			TBS::Nullable<cv::Mat> copy;
			{
				Poco::Mutex::ScopedLock l(m);
				copy = current;
			}

			cv::Mat small, med;
			if (copy.isSet()){
				cv::resize(copy.ref(), small, cv::Size(32,24));
				cv::resize(copy.ref(), med, cv::Size(48,32));

				//live stream
				Node::Data live(1, dt);

				live.set("image", streamcv(small));
				data.insert(std::make_pair("LiveStream", live));

				//alarm stream
				Node::Data alarm(0, dt);

				alarm.set("image", streamcv(med));
				data.insert(std::make_pair("AlarmStream", alarm));
			}
			return data;
		}


} /* namespace BB */
