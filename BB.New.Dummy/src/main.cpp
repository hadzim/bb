/*
 * main.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#include <BB/Node/Switch/SwitchNode.h>
#include "BB/App2.h"
#include "BB/Node/INode.h"
#include "BB/Node/Sensor/TemperatureNode.h"
#include "BB/Node/Sensor/MotionNode.h"
#include "BB/Node/Sensor/ContactNode.h"
#include "BB/Node/Status/StatusNode.h"
#include "BB/Node/Status/SecurityNode.h"
#include "BB/Node/Status/HardwareNode.h"
#include "BB/Node/Sensor/CameraNode.h"
#include "BB/Node/RemoteClient/RemoteClientNode.h"

#include "BB/Camera/OpenCVGrabber.h"

#include "TBS/Log.h"


namespace BB {
	namespace Dummy {

		class TempNode: public TemperatureNode {
			public:
				TempNode(std::string uid, int period, int start) :
						TemperatureNode(uid, period), val(start) {

				}
				BB::Node::Data readOne(const BB::Node::Info & info, const BB::Node::Sensor & sensor) {
					if (val > 100) {
						val = 0;
					}
					val++;
					BB::Node::Data data(val, BB::Node::localNow());
					return data;
				}
				double val;
		};

		class MotNode: public MotionNode {
			public:
				MotNode(std::string uid, int period, int start) :
						MotionNode(uid, period), val(start) {

				}
				BB::Node::Data readOne(const BB::Node::Info & info, const BB::Node::Sensor & sensor) {
					if (val > 100) {
						val = 0;
					}
					val++;
					BB::Node::Data data(val, BB::Node::localNow());
					return data;
				}
				double val;
		};

		class ConNode: public ContactNode {
			public:
				ConNode(std::string uid, int period, int start) :
						ContactNode(uid, period), val(start) {

				}
				BB::Node::Data readOne(const BB::Node::Info & info, const BB::Node::Sensor & sensor) {
					if (val > 100) {
						val = 0;
					}
					val++;
					BB::Node::Data data(val > 50 ? 1 : 0, BB::Node::localNow());
					return data;
				}
				double val;
		};


		class ConsoleSwitchNode: public SwitchNode {
			public:
			ConsoleSwitchNode(std::string uid, bool defaultVal) :
				SwitchNode(uid, defaultVal) {

			}

			virtual void performSwitch(bool on){
				LERROR("Switch") << "Switch " << this->getInfo().getUID() << " to " << (on ? 1 : 0) << LE;
			}
		};


		class Factory: public INodeFactory {
			private:

			public:
				virtual int getCheckingPeriodInMs() {
					return 60000;
				}
				virtual INode::PtrList getNodes() {
					INode::PtrList nodes;

					{
						INode::Ptr sensorNode = new TempNode("temp@1", 60 * 1000, 0);
						nodes.push_back(sensorNode);
					}
					{
						INode::Ptr sensorNode = new TempNode("temp@2", 60 * 1000, 30);
						nodes.push_back(sensorNode);
					}
					{
						INode::Ptr sensorNode = new TempNode("temp@3", 60 * 1000, 70);
						nodes.push_back(sensorNode);
					}

					{
						INode::Ptr sensorNode = new MotNode("motion@1", 60 * 1000, 0);
						nodes.push_back(sensorNode);
					}
					{
						INode::Ptr sensorNode = new MotNode("motion@2", 60 * 1000, 80);
						nodes.push_back(sensorNode);
					}

					{
						INode::Ptr sensorNode = new ConNode("contact@1", 60 * 1000, 0);
						nodes.push_back(sensorNode);
					}
					{
						INode::Ptr sensorNode = new ConNode("contact@2", 60 * 1000, 80);
						nodes.push_back(sensorNode);
					}

					{
						INode::Ptr sensorNode = new ConsoleSwitchNode("switch1", false);
						nodes.push_back(sensorNode);
					}
					{
						INode::Ptr sensorNode = new ConsoleSwitchNode("switch2", false);
						nodes.push_back(sensorNode);
					}


					/*{

						BB::Camera::OpenCVGrabber::Ptr g = new BB::Camera::OpenCVGrabber();
						g->open("rtsp://flash3.todostreaming.es/radiovida/mobile");
						INode::Ptr sensorNode = new CameraNode(g, "Cam1", 60 * 1000);
						nodes.push_back(sensorNode);
					}*/



					return nodes;
				}
		};

	}

} /* namespace BB */

NODE_BB_MAIN("DummyTemperature", BB::Dummy::Factory)

