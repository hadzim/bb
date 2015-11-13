/*
 * main.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */

#include "BB/App2.h"
#include "BB/Node/INode.h"
#include "BB/Node/Sensor/TemperatureNode.h"
#include "BB/Node/Sensor/MotionNode.h"
#include "BB/Node/Status/StatusNode.h"
#include "BB/Node/Status/HardwareNode.h"

std::string rightPin;
std::string armtopic;
std::string armed;
void onMessage(TBS::MQTT::Message & m){
	std::cout << "retrieve pin: " << m.payload << std::endl;
	if (m.topic.part(5) == "pin"){
		rightPin = m.payload;
	}
	if (m.topic.part(5) == "armed"){
		armed = m.payload;
		armtopic = m.topic.topic();
	}
}

int main(int argc, char **argv) {

	TBS::MQTT::Client client("arduino");
	client.OnMessage += Poco::delegate(onMessage);
	client.subscribe("devices/+/nodes/status/settings/pin");
	client.subscribe("devices/+/nodes/status/settings/armed");
	std::string s;
	std::cout << "please enter PIN" << std::endl;
	std::cin >> s;
	std::cout << "entered: " << s << std::endl;
	if (s == rightPin){
		//right pin - disarm / arm
		std::cout << "right pin" << std::endl;;

		client.publish(armtopic + "/new", armed == "0" ? "1" : "0");

	} else {
		std::cout << "wrong pin" << std::endl;;
	}

}
